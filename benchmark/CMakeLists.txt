set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language(Fortran)

set(
  CMAKE_Fortran_MODULE_DIRECTORY
  "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(inc)

add_executable(benchmark_hdc hdc_array_manipulation.cpp hdc_node_manipulation.cpp main.cpp)
target_link_libraries(benchmark_hdc benchmark hdc chdc )
install(TARGETS benchmark_hdc DESTINATION bin)

add_executable(benchmark_hdc_c hdc_array_manipulation_c.cpp hdc_node_manipulation_c.cpp main.cpp)
target_link_libraries(benchmark_hdc_c benchmark hdc chdc )
install(TARGETS benchmark_hdc_c DESTINATION bin)

add_executable(benchmark_fortran benchmark_fortran.f90 timer.c)
target_link_libraries(benchmark_fortran hdc chdc fhdc)
install(TARGETS benchmark_fortran DESTINATION bin)
set_target_properties(benchmark_fortran PROPERTIES LINK_FLAGS "-lc")

add_executable(test_hdc_get test_hdc_get.f90 timer.c)
target_link_libraries(test_hdc_get hdc chdc fhdc)
install(TARGETS test_hdc_get DESTINATION bin)
set_target_properties(test_hdc_get PROPERTIES LINK_FLAGS "-lc")

add_executable(test_hdc_get_all test_hdc_get_all.f90 timer.c)
target_link_libraries(test_hdc_get_all hdc chdc fhdc)
install(TARGETS test_hdc_get_all DESTINATION bin)
set_target_properties(test_hdc_get_all PROPERTIES LINK_FLAGS "-lc")

add_executable(test_hdc_put test_hdc_put.f90 timer.c)
target_link_libraries(test_hdc_put hdc chdc fhdc)
install(TARGETS test_hdc_put DESTINATION bin)
set_target_properties(test_hdc_put PROPERTIES LINK_FLAGS "-lc")

add_executable(test_hdc_put_all test_hdc_put_all.f90 timer.c)
target_link_libraries(test_hdc_put_all hdc chdc fhdc)
install(TARGETS test_hdc_put_all DESTINATION bin)
set_target_properties(test_hdc_put_all PROPERTIES LINK_FLAGS "-lc"
 )

add_executable(benchmark_paper benchmark_paper.cpp)
target_link_libraries(benchmark_paper benchmark hdc chdc)
install(TARGETS benchmark_paper DESTINATION bin)

