cmake_minimum_required(VERSION 3.12)
project(MyBPM)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)  # Add this line if not present

# Add the subdirectories
add_subdirectory(src)
# add_subdirectory(tests)
