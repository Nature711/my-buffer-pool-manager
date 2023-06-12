#pragma once

#include <cstdint>
#include <cstddef>

static constexpr int INVALID_PAGE_ID = -1;  // invalid page id
static constexpr int PAGE_SIZE = 4096;      // size of a data page in byte
static constexpr int BUFFER_POOL_SIZE = 10; // size of buffer pool

using frame_id_t = int32_t; // frame id type
using page_id_t = int32_t;  // page id type