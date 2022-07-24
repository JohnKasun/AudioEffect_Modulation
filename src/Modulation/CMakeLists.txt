add_subdirectory(lib_APFilter)
add_library(Modulation STATIC include/ModulationBase.h include/Modulation/ModulationIf.h ModulationBase.cpp ModulationIf.cpp)
target_include_directories(Modulation PUBLIC include/Modulation PRIVATE include)
target_link_libraries(Modulation PUBLIC Lfo RingBuffer lib_APFilter)
