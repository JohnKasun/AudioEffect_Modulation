add_subdirectory(lib_AudioFileIO)

add_executable(ModulationConsoleApp main.cpp)
target_link_libraries(ModulationConsoleApp
        PUBLIC
                lib_AudioFileIO
                lib_Lfo
                lib_RingBuffer
                lib_Modulation
)