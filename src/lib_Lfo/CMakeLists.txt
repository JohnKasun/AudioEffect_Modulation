add_library(lib_Lfo STATIC include/Lfo.h Lfo.cpp)
target_include_directories(lib_Lfo PUBLIC include)
target_link_libraries(lib_Lfo PUBLIC lib_RingBuffer lib_Wavetable)