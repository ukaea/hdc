add_library(hdc-jni SHARED java_hdc.cpp)

target_include_directories(hdc-jni PRIVATE /home/jhollocombe/miniconda3/include/linux)
target_link_libraries(hdc-jni PUBLIC hdc)

# handle SIGSEGV noprint