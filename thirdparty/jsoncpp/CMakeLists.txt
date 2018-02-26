project(JsonCpp)

set(jsoncpp_THIRD_PARTY 1)
set(jsoncpp_LIBRARIES jsoncpp)

set(JSONCPP_SOURCES
  jsoncpp.cpp
)

include_directories(${JSONCPP_SOURCE_DIR} ${JSONCPP_BINARY_DIR})

add_library(jsoncpp ${JSONCPP_SOURCES})
