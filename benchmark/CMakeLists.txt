set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")

add_executable(hdc_benchmark hdc_array_manipulation.cpp hdc_node_manipulation.cpp main.cpp)
target_link_libraries(hdc_benchmark benchmark hdc chdc )

add_executable(hdc_benchmark_c hdc_array_manipulation_c.cpp hdc_node_manipulation_c.cpp main.cpp)
target_link_libraries(hdc_benchmark_c benchmark hdc chdc )

add_executable(hdc_benchmark_fortran hdc_benchmark_fortran.f90 c_perf.f90 timer.c)
target_link_libraries(hdc_benchmark_fortran hdc chdc fhdc)
set_target_properties(hdc_benchmark_fortran PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_get hdc_benchmark_get.f90 c_perf.f90 timer.c)
target_link_libraries(hdc_benchmark_get hdc chdc fhdc)
set_target_properties(hdc_benchmark_get PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_get_all hdc_benchmark_get_all.f90 c_perf.f90 timer.c)
target_link_libraries(hdc_benchmark_get_all hdc chdc fhdc)
set_target_properties(hdc_benchmark_get_all PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_put hdc_benchmark_put.f90 c_perf.f90 timer.c)
target_link_libraries(hdc_benchmark_put hdc chdc fhdc)
set_target_properties(hdc_benchmark_put PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_put_all hdc_benchmark_put_all.f90 c_perf.f90  timer.c)
target_link_libraries(hdc_benchmark_put_all hdc chdc fhdc)
set_target_properties(hdc_benchmark_put_all PROPERTIES LINK_FLAGS "-lc")

add_executable(hdc_benchmark_paper hdc_benchmark_paper.cpp)
target_link_libraries(hdc_benchmark_paper benchmark hdc chdc)

install(TARGETS
        hdc_benchmark
        hdc_benchmark_c
        hdc_benchmark_fortran
        hdc_benchmark_get
        hdc_benchmark_get_all
        hdc_benchmark_put
        hdc_benchmark_put_all
        hdc_benchmark_paper
        DESTINATION bin)
