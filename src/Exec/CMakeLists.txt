set(APP ModulationExec)

add_subdirectory(AudioFileIO)

add_executable(${APP})

file(GLOB ${APP}_SOURCES *.cpp)
file(GLOB ${APP}_HEADERS include/*.h)

set(${APP}_DEPEND)
list(APPEND ${APP}_DEPEND AudioFileIO/include/AudioFileIO/AudioFileIf.h)
list(APPEND ${APP}_DEPEND ${CMAKE_SOURCE_DIR}/src/Modulation/include/Modulation/ModulationIf.h)

target_sources(${APP} 
        PRIVATE
                ${${APP}_SOURCES}
                ${${APP}_HEADERS}
                ${${APP}_DEPEND})

target_link_libraries(${APP}
        PUBLIC
                AudioFileIO
                Lfo
                RingBuffer
                Modulation
)