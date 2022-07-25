add_library(lib_RingBuffer 
    INTERFACE 
        include/RingBuffer.h
)

target_include_directories(lib_RingBuffer INTERFACE include/)