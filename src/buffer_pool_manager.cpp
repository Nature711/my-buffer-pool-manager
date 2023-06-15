#include "buffer_pool_manager.h"

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, ClockReplacer *clock_replacer)
    : pool_size_(pool_size),
      disk_manager_(disk_manager),
      clock_replacer_(clock_replacer)
{

    // allocate a consecutive memory space for buffer pool
    pages_ = new Page[pool_size_];

    // Initially, every page is in the free list
    for (size_t i = 0; i < pool_size_; ++i)
    {
        free_list_.emplace_back(static_cast<int>(i));
    }
}

BufferPoolManager::~BufferPoolManager()
{
    delete[] pages_;
}

/**
 * TODO(P1): Add implementation
 *
 * @brief Create a new page in the buffer pool. Set page_id to the new page's id, or nullptr if all frames
 * are currently in use and not evictable (in another word, pinned).
 *
 * You should pick the replacement frame from either the free list or the replacer (always find from the free list
 * first), and then call the AllocatePage() method to get a new page id. If the replacement frame has a dirty page,
 * you should write it back to the disk first. You also need to reset the memory and metadata for the new page.
 *
 * Remember to "Pin" the frame by calling replacer.SetEvictable(frame_id, false)
 * so that the replacer wouldn't evict the frame before the buffer pool manager "Unpin"s it.
 * Also, remember to record the access history of the frame in the replacer for the lru-k algorithm to work.
 *
 * @param[out] page_id id of created page
 * @return nullptr if no new pages could be created, otherwise pointer to new page
 */
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
            Page victim = pages_[frame_id];
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
    pages_[frame_id].ResetMemory();
    pages_[frame_id].page_id_ = *page_id;
    pages_[frame_id].pin_count_ = 1;
    pages_[frame_id].is_dirty_ = false;

    // Pin the frame and record the access history in the clock replacer
    clock_replacer_->Pin(frame_id);

    // Update the page table
    page_table_[*page_id] = frame_id;

    return &pages_[frame_id];
}

Page *BufferPoolManager::FetchPage(page_id_t page_id)
{
    // TODO: Implement FetchPage
    return nullptr;
}

bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty)
{
    // TODO: Implement UnpinPage
    return false;
}

bool BufferPoolManager::FlushPage(page_id_t page_id)
{
    // TODO: Implement FlushPage
    return false;
}

void BufferPoolManager::FlushAllPages()
{
    // TODO: Implement FlushAllPage
}

bool BufferPoolManager::DeletePage(page_id_t page_id)
{
    // TODO: Implement DeletePage
    return false;
}
