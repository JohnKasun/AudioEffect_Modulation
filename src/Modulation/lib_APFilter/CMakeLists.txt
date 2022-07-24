add_library(lib_APFilter STATIC include/APFilter.h APFilter.cpp)
target_include_directories(lib_APFilter PUBLIC include)
target_link_libraries(lib_APFilter PUBLIC RingBuffer)