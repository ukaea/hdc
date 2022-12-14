set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")

add_executable(hdc_test hdc_test_cpp.cpp main.cpp)
target_link_libraries(hdc_test hdc ${Boost_LIBRARIES})
add_test(NAME hdc_test COMMAND hdc_test WORKING_DIRECTORY "${hdc_BINARY_DIR}")
if (_BUILD_MDBM)
    target_compile_definitions(hdc_test PRIVATE -D_USE_MDBM=1)
endif()
install(TARGETS hdc_test DESTINATION bin)

add_executable(hdc_test_c hdc_test_c.cpp main.cpp)
target_link_libraries(hdc_test_c hdc chdc ${Boost_LIBRARIES})
add_test(NAME hdc_test_c COMMAND hdc_test_c WORKING_DIRECTORY "${hdc_BINARY_DIR}")
if (_BUILD_MDBM)
    target_compile_definitions(hdc_test_c PRIVATE -D_USE_MDBM=1)
endif()
install(TARGETS hdc_test_c DESTINATION bin)

if (_BUILD_MDBM)
    add_executable(hdc_test_serialization hdc_test_serialization.cpp)
    target_link_libraries(hdc_test_serialization hdc ${Boost_LIBRARIES})
    add_test(NAME hdc_test_serialization COMMAND hdc_test_serialization WORKING_DIRECTORY "${hdc_BINARY_DIR}")
    target_compile_definitions(hdc_test_serialization PRIVATE -D_USE_MDBM=1)
    install(TARGETS hdc_test_serialization DESTINATION bin)
endif()

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "DEBUG" AND ENABLE_COVERAGE)
    set(COVERAGE_GCOVR_EXCLUDES '../build/*' '../thirdparty/*' '../examples/*' '../test/*')
    SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME test_coverage EXECUTABLE ctest DEPENDENCIES hdc_test)
endif()
