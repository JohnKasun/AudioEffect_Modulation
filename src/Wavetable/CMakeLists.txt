set(CurrLib Wavetable)

file(GLOB ${CurrLib}_SOURCES Wavetable.cpp)
file(GLOB ${CurrLib}_HEADERS include/*.h)

set(${CurrLib}_DEPEND)

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