set(SRC ${CMAKE_SOURCE_DIR}/src)
add_executable(unit_tests
                ${SRC}/Modulation/testModulation.cpp
                ${SRC}/Lfo/testLfo.cpp
                ${SRC}/Wavetable/testWavetable.cpp)

target_include_directories(unit_tests PRIVATE include)

target_link_libraries(unit_tests
    PRIVATE
        Lfo
        Modulation
        gtest_main
)

add_subdirectory(googletest)
include(GoogleTest)
gtest_discover_tests(unit_tests)