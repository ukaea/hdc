project(JsonCpp)

set(hdcjsoncpp_THIRD_PARTY 1)
set(hdcjsoncpp_LIBRARIES hdcjsoncpp)

set(JSONCPP_SOURCES
  jsoncpp.cpp
)

include_directories(${JSONCPP_SOURCE_DIR} ${JSONCPP_BINARY_DIR})

add_library(hdcjsoncpp ${JSONCPP_SOURCES})
