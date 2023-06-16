#include "buffer_pool_manager.h"
#include "clock_replacer.h"

#include <iostream>
#include <string>

int main()
{
    std::string filename = "database.db";
    DiskManager disk_manager(filename);
    ClockReplacer clock_replacer(BUFFER_POOL_SIZE);
    BufferPoolManager buffer_pool_manager(BUFFER_POOL_SIZE, &disk_manager, &clock_replacer);

    // new page
    for (int i = 0; i < 10; ++i)
    {
        page_id_t page_id;
        Page *page = buffer_pool_manager.NewPage(&page_id);
        if (page != nullptr)
        {
            std::cout << "Allocated page: " << page_id << std::endl;
            std::cout << "Pin Count: " << page->GetPinCount() << std::endl;
            std::cout << "Is Dirty: " << (page->IsDirty() ? "true" : "false") << std::endl;
        }
        else
        {
            std::cout << "Failed to allocate a new page." << std::endl;
        }
    }

    // fetch page
    for (int i = 0; i < 10; ++i)
    {
        page_id_t page_id = i; // Replace with the desired page ID
        Page *page = buffer_pool_manager.FetchPage(page_id);

        // Check if the page is fetched successfully
        if (page != nullptr)
        {
            // Print the fetched page's information
            std::cout << "Fetched Page ID: " << page->GetPageId() << std::endl;
            std::cout << "Fetched Page Data: " << page->GetData() << std::endl;
        }
        else
        {
            std::cout << "Page not found or unavailable." << std::endl;
        }
    }

    return 0;
}
