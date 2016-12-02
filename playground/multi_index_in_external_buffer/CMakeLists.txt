cmake_minimum_required(VERSION 2.6)
project(multi_index_in_magaged_heap)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(multi_index_in_external_buffer main.cpp)
find_package(Boost 1.48.0 REQUIRED system)

install(TARGETS multi_index_in_external_buffer RUNTIME DESTINATION bin)
