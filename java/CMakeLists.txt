cmake_minimum_required( VERSION 3.3 )

########################################################################################################################
# CMake configuration
cmake_policy( SET CMP0057 NEW )

# Switch between standalone and embedded build
if (ENABLE_JAVA)
  message("ENABLE_JAVA=TRUE => performing embedded build")
  set ( jhdc_SOURCE_DIR "${hdc_SOURCE_DIR}" )
  set ( jhdc_BINARY_DIR "${hdc_BINARY_DIR}/java" )
else()
  project( jhdc VERSION 0.20.0 )
  message("ENABLE_JAVA=FALSE => performing standalone build")

  find_package( PkgConfig )
  pkg_check_modules( HDC hdc=0.20.0 REQUIRED )

  # Build options
  option( INSTALL_JAVA_DEPENDENCIES "Install jHDC JAR dependencies" OFF )
  option( JAR_WITH_DEPENDENCIES "Bundle all dependencies into a single jar - the result can be large (~80 MB)" OFF )
  set( JAVACPP_PLATFORM "linux-x86_64" CACHE STRING "Strip unnecessary native libraries from bundled jar. Only relevant when JAR_WITH_DEPENDENCIES=ON" )
  set( JAR_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share/java" CACHE STRING "JAR install prefix" )
endif(ENABLE_JAVA)

find_package(JNI)
find_package(Java)

include(AddFileDependencies)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

# copy all from java directory
message("--------   cp ${CMAKE_CURRENT_SOURCE_DIR}/java_hdc.cpp ${jhdc_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/java_hdc.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/jHDC" DESTINATION "${jhdc_BINARY_DIR}")


# generate native headers - we can not use jni UseJava since at this time it supports only java 11
add_custom_command(OUTPUT ${jhdc_BINARY_DIR}/dev_libhdc_HDC.h
    COMMAND javah -classpath jHDC/src/main/java -jni dev.libhdc.HDC
)
add_file_dependencies(java_hdc.cpp ${jhdc_BINARY_DIR}/dev_libhdc_HDC.h)
add_library(hdc-jni SHARED java_hdc.cpp)
target_include_directories(hdc-jni PRIVATE ${JNI_INCLUDE_DIRS} ${jhdc_BINARY_DIR})
if ( ENABLE_JAVA )
  target_link_libraries(hdc-jni PUBLIC hdc)
else()
  target_include_directories(hdc-jni PRIVATE ${HDC_INCLUDE_DIRS})
  target_link_directories( hdc-jni PUBLIC ${HDC_LIBRARY_DIRS} )
  target_link_libraries(hdc-jni PUBLIC ${HDC_LIBRARIES})
endif( ENABLE_JAVA )
install( TARGETS hdc-jni LIBRARY DESTINATION lib )

add_subdirectory(jHDC)
