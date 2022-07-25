set(LIB lib_Wavetable)
add_library(${LIB} STATIC include/Wavetable.h Wavetable.cpp)
target_include_directories(${LIB} PUBLIC include)