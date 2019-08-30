find_package(JNI)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

add_library(hdc-jni SHARED java_hdc.cpp)

target_include_directories(hdc-jni PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(hdc-jni PUBLIC hdc)

# handle SIGSEGV noprint
