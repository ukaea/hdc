file(GLOB Pluma_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include/Pluma/*)
file(GLOB Pluma_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/src/Pluma/*)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/src)
add_library(Pluma STATIC ${Pluma_SOURCES})
