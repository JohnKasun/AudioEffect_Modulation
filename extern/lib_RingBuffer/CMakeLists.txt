set(LIB lib_RingBuffer)
add_library(${LIB} 
    INTERFACE 
        include/RingBuffer.h
)

target_include_directories(${LIB}  INTERFACE include/)