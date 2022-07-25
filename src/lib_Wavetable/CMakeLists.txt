
add_library(lib_Wavetable STATIC include/Wavetable.h Wavetable.cpp)
target_include_directories(lib_Wavetable PUBLIC include)