set(TEST_FILES
        lib_Chorus/test/testChorus.cpp 
        lib_Lfo/test/testLfo.cpp 
        lib_Wavetable/test/testWavetable.cpp
        lib_Chorus/lib_APFilter/test/testAPFilter.cpp)
list(TRANSFORM TEST_FILES PREPEND "${CMAKE_SOURCE_DIR}/src/")

set(APP app_UnitTests)
add_executable(${APP} ${TEST_FILES})
target_include_directories(${APP} PRIVATE include)
target_link_libraries(${APP}
    PRIVATE
        lib_Lfo
        lib_Chorus
        gtest_main
)

add_subdirectory(googletest)
include(GoogleTest)
gtest_discover_tests(${APP})