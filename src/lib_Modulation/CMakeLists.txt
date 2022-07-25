add_subdirectory(lib_APFilter)

set(LIB lib_Modulation)
add_library(${LIB} STATIC include/ModulationBase.h include/${LIB}/ModulationIf.h ModulationBase.cpp ModulationIf.cpp)
target_include_directories(${LIB} PUBLIC include/${LIB} PRIVATE include)
target_link_libraries(${LIB} PUBLIC lib_Lfo lib_RingBuffer lib_APFilter)
