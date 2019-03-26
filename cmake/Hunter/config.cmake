hunter_config(Boost
    VERSION ${HUNTER_Boost_VERSION}
    CMAKE_ARGS BUILD_SHARED_LIBS=ON 
)
hunter_config(jsoncpp
    VERSION ${HUNTER_jsoncpp_VERSION}
    CMAKE_ARGS BUILD_SHARED_LIBS=ON
)
