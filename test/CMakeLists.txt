set(SRC ${CMAKE_SOURCE_DIR}/src)
add_executable(unit_tests
                ${SRC}/lib_Modulation/testModulation.cpp
                ${SRC}/lib_Lfo/testLfo.cpp
                ${SRC}/lib_Wavetable/testWavetable.cpp)

target_include_directories(unit_tests PRIVATE include)

target_link_libraries(unit_tests
    PRIVATE
        lib_Lfo
        lib_Modulation
        gtest_main
)

add_subdirectory(googletest)
include(GoogleTest)
gtest_discover_tests(unit_tests)