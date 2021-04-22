find_package(JNI)
find_package(Java)

include(AddFileDependencies)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

# copy all from java directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION "${hdc_BINARY_DIR}")

# generate native headers - we can not use jni UseJava since at this time it supports only java 11
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dev_libhdc_HDC.h
    COMMAND javah -classpath jHDC/src/main/java -jni dev.libhdc.HDC
)
add_file_dependencies(java_hdc.cpp ${CMAKE_CURRENT_BINARY_DIR}/dev_libhdc_HDC.h)
add_library(hdc-jni SHARED java_hdc.cpp)

target_include_directories(hdc-jni PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}) 
target_link_libraries(hdc-jni PUBLIC hdc)
install( TARGETS hdc-jni LIBRARY DESTINATION lib )

add_subdirectory(jHDC)
