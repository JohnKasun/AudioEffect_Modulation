add_library(Lfo STATIC include/Lfo.h Lfo.cpp)
target_include_directories(Lfo PUBLIC include)
target_link_libraries(Lfo PUBLIC RingBuffer Wavetable)