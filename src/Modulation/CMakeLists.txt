set(CurrLib Modulation)

file(GLOB ${CurrLib}_SOURCES ModulationIf.cpp ModulationBase.cpp APFilter.cpp)
file(GLOB ${CurrLib}_HEADERS include/Modulation/ModulationIf.h include/ModulationBase.h include/APFilter.h)

set(${CurrLib}_DEPEND)
list(APPEND ${CurrLib}_DEPEND ${CMAKE_SOURCE_DIR}/src/Lfo/include/Lfo.h)
list(APPEND ${CurrLib}_DEPEND ${CMAKE_SOURCE_DIR}/extern/RingBuffer/include/RingBuffer.h)


add_library(${CurrLib} STATIC ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS} ${${CurrLib}_DEPEND})
target_include_directories(${CurrLib} PUBLIC include/${CurrLib} PRIVATE include)

set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${CurrLib} PROPERTIES OUTPUT_NAME "${CurrLib}")
set_target_properties(${CurrLib} PROPERTIES PREFIX "lib")

target_link_libraries(${CurrLib}
        PUBLIC
                Lfo
                RingBuffer
)