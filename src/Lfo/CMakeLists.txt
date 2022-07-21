set(CurrLib Lfo)

file(GLOB ${CurrLib}_SOURCES Lfo.cpp)
file(GLOB ${CurrLib}_HEADERS include/*.h)

set(${CurrLib}_DEPEND)
list(APPEND ${CurrLib}_DEPEND ${CMAKE_SOURCE_DIR}/extern/RingBuffer/include/RingBuffer.h)
list(APPEND ${CurrLib}_DEPEND ${CMAKE_SOURCE_DIR}/src/Wavetable/include/Wavetable.h)

add_library(${CurrLib}
        STATIC 
                ${${CurrLib}_SOURCES}
                ${${CurrLib}_HEADERS}
                ${${CurrLib}_DEPEND}
)

target_include_directories(${CurrLib}
    PUBLIC
        include/
)

target_link_libraries(${CurrLib}
        PUBLIC
                RingBuffer
                Wavetable
)