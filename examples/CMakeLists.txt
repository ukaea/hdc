set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")

add_executable(hdc_example_cpp example_cpp.cpp)
target_link_libraries(hdc_example_cpp hdc)

add_executable(hdc_example_c example_c.c)
target_link_libraries(hdc_example_c chdc)

add_executable(show_type_traits show_type_traits.cpp)
target_link_libraries(show_type_traits hdc)

set(HDC_EXAMPLES hdc_example_cpp hdc_example_c show_type_traits)

if (_BUILD_MDBM)
    add_executable(hdc_example_interprocess example_interprocess.cpp)
    if( APPLE )
    target_link_libraries(hdc_example_interprocess hdc)
    else()
    target_link_libraries(hdc_example_interprocess hdc rt)
    endif()
    list(APPEND HDC_EXAMPLES hdc_example_interprocess)
endif (_BUILD_MDBM)

if(ENABLE_REDIS)
    add_executable(hdc_example_redis example_redis.cpp)
    if( APPLE )
    target_link_libraries(hdc_example_redis hdc)
    else()
    target_link_libraries(hdc_example_redis hdc rt)
    endif()
    list(APPEND HDC_EXAMPLES hdc_example_redis)
endif(ENABLE_REDIS)

if(ENABLE_UDA)
    add_executable(hdc_example_uda example_uda.cpp)
    target_link_libraries(hdc_example_uda hdc)
    install(TARGETS hdc_example_uda
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
endif(ENABLE_UDA)

install(TARGETS ${HDC_EXAMPLES}
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
