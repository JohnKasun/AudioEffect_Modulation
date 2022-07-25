set(LIB lib_Lfo)
add_library(${LIB} STATIC include/Lfo.h Lfo.cpp)
target_include_directories(${LIB} PUBLIC include)
target_link_libraries(${LIB} PUBLIC lib_RingBuffer lib_Wavetable)