# Fetch and include the Google Test framework
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(googletest)

# Create the test executable and link with the buffer pool manager library
add_executable(BufferPoolManagerTest buffer_pool_manager_test.cpp)
target_link_libraries(BufferPoolManagerTest gtest gtest_main buffer_pool_manager)

# Include the tests as a build target
enable_testing()
add_test(NAME BufferPoolManagerTest COMMAND BufferPoolManagerTest)
