#include "buffer_pool_manager.h"
#include "clock_replacer.h"

#include <iostream>
#include <string>

int main()
{
    std::string filename = "database.db";
    std::fstream file(filename, std::ios::binary | std::ios::out);
    if (!file)
    {
        std::cerr << "Failed to create database file." << std::endl;
        return 1;
    }

    DiskManager disk_manager(filename);
    ClockReplacer clock_replacer(BUFFER_POOL_SIZE);
    BufferPoolManager buffer_pool_manager(BUFFER_POOL_SIZE, &disk_manager, &clock_replacer);

    std::cout << "Hello, world!" << std::endl;

    file.close();
    return 0;
}
