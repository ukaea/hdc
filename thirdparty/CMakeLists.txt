file(GLOB Pluma_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include/Pluma/*)
file(GLOB Pluma_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/src/Pluma/*)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/src)
add_library(Pluma STATIC ${Pluma_SOURCES})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Pluma-1.1/include/Pluma/ DESTINATION include/hdc/Pluma)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/json DESTINATION include/hdc)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/andres/ DESTINATION include/hdc/andres)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CLI/ DESTINATION include/hdc/CLI)
