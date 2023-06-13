#include "buffer_pool_manager.h"

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, ClockReplacer *clock_replacer)
    : pool_size_(pool_size),
      disk_manager_(disk_manager)
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

Page *BufferPoolManager::NewPage(page_id_t *page_id)
{
    // TODO: Implement NewPage
    return nullptr;
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
