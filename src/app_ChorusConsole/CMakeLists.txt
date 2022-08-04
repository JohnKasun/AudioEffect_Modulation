add_subdirectory(lib_AudioFileIO)

set(APP app_ChorusConsole)
add_executable(${APP} main.cpp)
target_link_libraries(${APP}
        PUBLIC
                lib_AudioFileIO
                lib_Lfo
                lib_RingBuffer
                lib_Chorus
)