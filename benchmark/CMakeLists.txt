set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)

set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(inc)

file(GLOB SRCS *.cpp)
add_executable(benchmark_hdc ${SRCS})
target_link_libraries(benchmark_hdc benchmark hdc chdc )
install(TARGETS benchmark_hdc DESTINATION bin)

add_executable(benchmark_fortran benchmark_fortran.f90)
target_link_libraries(benchmark_fortran hdc chdc fhdc)
install(TARGETS benchmark_fortran DESTINATION bin)

add_executable(test_hdc_get test_hdc_get.f90 timer.c)
target_link_libraries(test_hdc_get hdc chdc fhdc)
install(TARGETS test_hdc_get DESTINATION bin)
set_target_properties(test_hdc_get PROPERTIES LINK_FLAGS "-lc")
