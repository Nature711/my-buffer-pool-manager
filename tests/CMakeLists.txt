# Set the path to the locally downloaded Google Test source directory
set(GTEST_SOURCE_DIR "googletest/")

# Include the Google Test framework
add_subdirectory(${GTEST_SOURCE_DIR})

# Create the test executable and link with the buffer pool manager library
add_executable(BufferPoolManagerTest buffer_pool_manager_test.cpp)

# Include the necessary directories
target_include_directories(BufferPoolManagerTest PRIVATE ${GTEST_SOURCE_DIR}/googletest/include ${GTEST_SOURCE_DIR}/googlemock/include)

# Link with the Google Test and Google Test Main libraries
target_link_libraries(BufferPoolManagerTest gtest gtest_main buffer_pool_manager)

# Include the tests as a build target
enable_testing()
add_test(NAME BufferPoolManagerTest COMMAND BufferPoolManagerTest)
