set(LIB lib_APFilter)
add_library(${LIB} STATIC include/APFilter.h APFilter.cpp)
target_include_directories(${LIB} PUBLIC include)
target_link_libraries(${LIB} PUBLIC lib_RingBuffer)