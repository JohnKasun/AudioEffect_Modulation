cmake_minimum_required(VERSION 3.15)
Project(JohnK VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
add_subdirectory(src bin)
add_subdirectory(cmake)
add_subdirectory(test)
add_subdirectory(extern)

