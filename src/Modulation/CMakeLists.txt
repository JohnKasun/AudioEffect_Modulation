set(CurrLib Modulation)

file(GLOB ${CurrLib}_SOURCES *.cpp;*.h)
file(GLOB ${CurrLib}_HEADERS include/*.h)

file(GLOB ${CurrLib}_UTILITY ${CMAKE_SOURCE_DIR}/extern/Utility/include/*.h)

set(${CurrLib}_DEPEND)
list(APPEND ${CurrLib}_DEPEND ${${CurrLib}_UTILITY})
list(APPEND ${CurrLib}_DEPEND ${CMAKE_SOURCE_DIR}/src/Lfo/include/Lfo.h)
list(APPEND ${CurrLib}_DEPEND ${CMAKE_SOURCE_DIR}/extern/RingBuffer/include/RingBuffer.h)


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
                Utility
                Lfo
                RingBuffer
)