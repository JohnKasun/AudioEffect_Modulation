include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

include(GoogleTest)
gtest_discover_tests(unit_tests)