#pragma once

#include <fstream>
#include <string>

#include "config.h"

/**                                                                                                                      \
 * DiskManager takes care of the allocation and deallocation of pages within a database. It performs the reading and     \
 * writing of pages to and from disk, providing a logical file layer within the context of a database management system. \
 */
class DiskManager
{
public:
    explicit DiskManager(const std::string &filename);
    ~DiskManager();

    bool ReadPage(page_id_t page_id, char *buffer);
    bool WritePage(page_id_t page_id, const char *buffer);

private:
    std::fstream file_;
};