# Add the source files
set(SOURCES main.cpp buffer_pool_manager.cpp clock_replacer.cpp)

# Create the library target
add_library(buffer_pool_manager ${SOURCES})

# Create the executable target
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Include the necessary directories
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)