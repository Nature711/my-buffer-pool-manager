#include "buffer_pool_manager.h"

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, ClockReplacer *clock_replacer)
    : pool_size_(pool_size),
      disk_manager_(disk_manager),
      clock_replacer_(clock_replacer)
{

    // allocate a consecutive memory space for buffer pool
    buffer_pool_pages_ = new Page[pool_size_];

    // Initially, every page is in the free list
    for (size_t i = 0; i < pool_size_; ++i)
    {
        free_list_.emplace_back(static_cast<int>(i));
    }
}

BufferPoolManager::~BufferPoolManager()
{
    delete[] buffer_pool_pages_;
}

Page *BufferPoolManager::NewPage(page_id_t *page_id)
{
    std::lock_guard<std::mutex> lock(latch_);
    frame_id_t frame_id;

    // try to find a free page from free list
    if (!free_list_.empty())
    {
        frame_id = free_list_.front();
        free_list_.pop_front();
    }
    else
    {
        // if free list is empty, try to find a victim frame from replacer
        if (!clock_replacer_->Victim(&frame_id))
        {
            // if no replacement frame is available, return nullptr
            *page_id = INVALID_PAGE_ID;
            return nullptr;
        }
        else
        {
            Page &victim = buffer_pool_pages_[frame_id];
            if (victim.IsDirty())
            {
                disk_manager_->WritePage(victim.GetPageId(), victim.GetData());
            }
            page_table_.erase(victim.GetPageId());
        }
    }

    // Allocate a new page id
    *page_id = AllocatePage();
    // Reset the memory and metadata of the new page
    Page &new_page = buffer_pool_pages_[frame_id];
    new_page.ResetMemory();
    new_page.page_id_ = *page_id;
    new_page.pin_count_ = 1;
    new_page.is_dirty_ = false;

    // Pin the frame and record the access history in the clock replacer
    clock_replacer_->Pin(frame_id);

    // Update the page table
    page_table_[*page_id] = frame_id;

    return &new_page;
}

Page *BufferPoolManager::FetchPage(page_id_t page_id)
{

    std::lock_guard<std::mutex> lock(latch_);

    // Check if the page is already in the buffer pool
    auto it = page_table_.find(page_id);
    if (it != page_table_.end())
    {
        // Page found in the buffer pool, get the frame ID
        frame_id_t frame_id = it->second;

        // Disable eviction and record the access history of the frame
        clock_replacer_->Pin(frame_id);

        // Return a pointer to the requested page
        return &buffer_pool_pages_[frame_id];
    }

    frame_id_t frame_id;
    if (!free_list_.empty())
    {
        frame_id = free_list_.front();
        free_list_.pop_front();
    }
    else
    {
        if (!clock_replacer_->Victim(&frame_id))
        {
            // if no replacement frame is available, return nullptr
            return nullptr;
        }
        else
        {
            // Flush the evicted page to disk if it's dirty
            Page &victim = buffer_pool_pages_[frame_id];
            if (victim.IsDirty())
            {
                disk_manager_->WritePage(victim.GetPageId(), victim.GetData());
            }
            page_table_.erase(victim.GetPageId());
        }
    }

    Page &target = buffer_pool_pages_[frame_id];
    // Read the requested page from disk
    disk_manager_->ReadPage(page_id, target.GetData());

    // Reset the memory and metadata of the new page
    target.page_id_ = page_id;
    target.pin_count_ = 1;
    target.is_dirty_ = false;

    // Pin the frame and record the access history in the clock replacer
    clock_replacer_->Pin(frame_id);
    // Update the page table
    page_table_[page_id] = frame_id;
    // Return a pointer to the requested page
    return &target;
}

bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty)
{
    std::lock_guard<std::mutex> lock(latch_);

    // Check if the page is already in the buffer pool
    auto it = page_table_.find(page_id);
    if (it == page_table_.end())
    {
        return false;
    }

    // Page found in the buffer pool, get the frame ID
    frame_id_t frame_id = it->second;
    Page &target = buffer_pool_pages_[frame_id];
    if (target.GetPinCount() <= 0)
    {
        return false;
    }

    // If pin count becomes 0 after decrementing, make the frame evictable by the replacer
    target.pin_count_ = target.pin_count_ - 1;
    if (target.pin_count_ == 0)
    {
        clock_replacer_->Unpin(frame_id);
    }
    target.is_dirty_ = is_dirty;

    return true;
}

bool BufferPoolManager::FlushPage(page_id_t page_id)
{
    std::lock_guard<std::mutex> lock(latch_);

    // Check if the page is already in the buffer pool
    auto it = page_table_.find(page_id);
    if (it == page_table_.end())
    {
        return false;
    }

    // Page found in the buffer pool, get the frame ID
    frame_id_t frame_id = it->second;
    Page &target = buffer_pool_pages_[frame_id];
    disk_manager_->WritePage(page_id, target.GetData());
    target.is_dirty_ = false;

    return true;
}

void BufferPoolManager::FlushAllPages()
{
    std::lock_guard<std::mutex> lock(latch_);

    for (size_t i = 0; i < pool_size_; ++i)
    {
        Page &page = buffer_pool_pages_[i];
        FlushPage(page.GetPageId());
    }
}

bool BufferPoolManager::DeletePage(page_id_t page_id)
{
    std::lock_guard<std::mutex> lock(latch_);

    // Check if the page is already in the buffer pool
    auto it = page_table_.find(page_id);
    if (it == page_table_.end())
    {
        return true;
    }
    // Page found in the buffer pool, get the frame ID
    frame_id_t frame_id = it->second;
    Page &target = buffer_pool_pages_[frame_id];
    if (target.GetPinCount() > 0)
    {
        return false;
    }
    page_table_.erase(target.GetPageId());

    clock_replacer_->RemoveFromReplacer(frame_id);

    free_list_.push_back(frame_id);

    target.ResetMemory();
    target.page_id_ = INVALID_PAGE_ID;
    target.pin_count_ = 0;
    target.is_dirty_ = false;

    return true;
}
