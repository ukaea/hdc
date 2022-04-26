set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${fhdc_BINARY_DIR}/mod")
link_directories("${fhdc_BINARY_DIR}/gbench_build/src/")
include_directories("${fhdc_BINARY_DIR}/gbench_src/include/")

add_library(c_perf STATIC c_perf.f90 timer.c)

add_executable(hdc_benchmark_fortran hdc_benchmark_fortran.f90)
target_link_libraries(hdc_benchmark_fortran hdc chdc fhdc c_perf)
set_target_properties(hdc_benchmark_fortran PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_get hdc_benchmark_get.f90)
target_link_libraries(hdc_benchmark_get hdc chdc fhdc c_perf)
set_target_properties(hdc_benchmark_get PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_get_all hdc_benchmark_get_all.f90)
target_link_libraries(hdc_benchmark_get_all hdc chdc fhdc c_perf)
set_target_properties(hdc_benchmark_get_all PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_put hdc_benchmark_put.f90)
target_link_libraries(hdc_benchmark_put hdc chdc fhdc c_perf)
set_target_properties(hdc_benchmark_put PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_put_all hdc_benchmark_put_all.f90)
target_link_libraries(hdc_benchmark_put_all hdc chdc fhdc c_perf)
set_target_properties(hdc_benchmark_put_all PROPERTIES LINK_FLAGS "-lc")

install(TARGETS
        hdc_benchmark_fortran
        hdc_benchmark_get
        hdc_benchmark_get_all
        hdc_benchmark_put
        hdc_benchmark_put_all
        DESTINATION bin)
