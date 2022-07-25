add_subdirectory(lib_APFilter)
add_library(lib_Modulation STATIC include/ModulationBase.h include/Modulation/ModulationIf.h ModulationBase.cpp ModulationIf.cpp)
target_include_directories(lib_Modulation PUBLIC include/Modulation PRIVATE include)
target_link_libraries(lib_Modulation PUBLIC lib_Lfo lib_RingBuffer lib_APFilter)
