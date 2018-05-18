file(GLOB Pluma_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include/Pluma/*)
file(GLOB Pluma_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/src/Pluma/*)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/src)
add_library(Pluma STATIC ${Pluma_SOURCES})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include/Pluma/ DESTINATION include/hdc/Pluma)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/json DESTINATION include/hdc/jsoncpp)
install(DIRECTORY ./jsoncpp/json DESTINATION include/hdc/jsoncpp FILES_MATCHING PATTERN "*.h")
