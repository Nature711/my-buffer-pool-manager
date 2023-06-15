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
    return 0;
}
