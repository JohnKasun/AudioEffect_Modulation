add_subdirectory(lib_sndlib)

add_library(lib_AudioFileIO STATIC AudioFile.cpp AudioFileIf.cpp)
target_include_directories(lib_AudioFileIO PUBLIC include/lib_AudioFileIO PRIVATE include)
target_link_libraries(lib_AudioFileIO PUBLIC lib_sndlib)
