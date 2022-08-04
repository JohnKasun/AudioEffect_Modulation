add_subdirectory(lib_APFilter)

set(LIB lib_Chorus)
add_library(${LIB} STATIC include/Chorus.h Chorus.cpp)
target_include_directories(${LIB} PUBLIC include)
target_link_libraries(${LIB} PUBLIC lib_Lfo lib_RingBuffer lib_APFilter)
