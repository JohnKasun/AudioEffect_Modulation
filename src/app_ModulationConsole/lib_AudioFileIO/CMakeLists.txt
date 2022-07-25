add_subdirectory(lib_sndlib)

set(LIB lib_AudioFileIO)
add_library(${LIB} STATIC AudioFile.cpp AudioFileIf.cpp)
target_include_directories(${LIB} PUBLIC include/${LIB} PRIVATE include)
target_link_libraries(${LIB} PUBLIC lib_sndlib)
