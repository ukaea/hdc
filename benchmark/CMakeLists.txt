set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_BINARY_DIR}/mod")
link_directories("${hdc_BINARY_DIR}/gbench_build/src/")
include_directories("${hdc_BINARY_DIR}/gbench_src/include/")
add_executable(hdc_benchmark hdc_array_manipulation.cpp hdc_node_manipulation.cpp main.cpp)
target_link_libraries(hdc_benchmark benchmark hdc chdc )

add_executable(hdc_benchmark_c hdc_array_manipulation_c.cpp hdc_node_manipulation_c.cpp main.cpp)
target_link_libraries(hdc_benchmark_c benchmark hdc chdc )

add_executable(hdc_benchmark_paper hdc_benchmark_paper.cpp)
target_link_libraries(hdc_benchmark_paper benchmark hdc chdc)

install(TARGETS
        hdc_benchmark
        hdc_benchmark_c
        hdc_benchmark_paper
        DESTINATION bin)
