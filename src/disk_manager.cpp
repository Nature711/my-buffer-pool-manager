#include "disk_manager.h"

DiskManager::DiskManager(const std::string &filename)
{
    file_.open(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
    if (!file_)
    {
        // Handle error, such as file not found or unable to open
    }
}

DiskManager::~DiskManager()
{
    file_.close();
}

bool DiskManager::ReadPage(page_id_t page_id, char *buffer)
{
    const std::streampos offset = static_cast<std::streampos>(page_id) * PAGE_SIZE;
    file_.seekg(offset);
    file_.read(buffer, PAGE_SIZE);
    return !file_.fail();
}

bool DiskManager::WritePage(page_id_t page_id, const char *buffer)
{
    const std::streampos offset = static_cast<std::streampos>(page_id) * PAGE_SIZE;
    file_.seekp(offset);
    file_.write(buffer, PAGE_SIZE);
    return !file_.fail();
}
