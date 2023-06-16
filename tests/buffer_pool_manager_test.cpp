#include "buffer_pool_manager.h"
#include <gtest/gtest.h>

TEST(BufferPoolManagerTest, NewPageTest)
{
    // Create an instance of the buffer pool manager
    std::string filename = "database.db";
    DiskManager disk_manager(filename);
    ClockReplacer clock_replacer(BUFFER_POOL_SIZE);
    BufferPoolManager buffer_pool_manager(BUFFER_POOL_SIZE, &disk_manager, &clock_replacer);

    // Perform test operations
    page_id_t page_id;
    Page *page;
    for (int i = 0; i < 10; ++i)
    {
        page = buffer_pool_manager.NewPage(&page_id);
        // Assert that the page allocation is successful
        ASSERT_NE(page, nullptr);
        // Print the page information
        std::cout << "Allocated page: " << page_id << std::endl;
        std::cout << "Pin Count: " << page->GetPinCount() << std::endl;
        std::cout << "Is Dirty: " << (page->IsDirty() ? "true" : "false") << std::endl;
    }
}

TEST(BufferPoolManagerTest, FetchPageTest)
{
    // Create an instance of the buffer pool manager
    std::string filename = "database.db";
    DiskManager disk_manager(filename);
    ClockReplacer clock_replacer(BUFFER_POOL_SIZE);
    BufferPoolManager buffer_pool_manager(BUFFER_POOL_SIZE, &disk_manager, &clock_replacer);

    // Call FetchPage
    page_id_t page_id = 1; // Replace with the desired page ID
    Page *page = buffer_pool_manager.FetchPage(page_id);

    // Check if the page is fetched successfully
    ASSERT_NE(page, nullptr);

    // Print the fetched page's information
    std::cout << "Fetched Page ID: " << page->GetPageId() << std::endl;
    std::cout << "Fetched Page Data: " << page->GetData() << std::endl;
}

TEST(BufferPoolManagerTest, UnpinPageTest)
{
    // Create a buffer pool manager instance
    std::string filename = "database.db";
    DiskManager disk_manager(filename);
    ClockReplacer clock_replacer(BUFFER_POOL_SIZE);
    BufferPoolManager buffer_pool_manager(BUFFER_POOL_SIZE, &disk_manager, &clock_replacer);

    // Create a new page
    page_id_t page_id;
    Page *new_page = buffer_pool_manager.NewPage(&page_id);
    // Perform operations on the new page

    // Unpin the page
    bool is_dirty = true;
    bool result = buffer_pool_manager.UnpinPage(page_id, is_dirty);

    // Check the result
    EXPECT_TRUE(result);
}

int main(int argc, char **argv)
{
    // Initialize the Google Test framework
    ::testing::InitGoogleTest(&argc, argv);

    // Run all tests
    return RUN_ALL_TESTS();
}
