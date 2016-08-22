set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)

set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

file(GLOB SRCS *.cpp)
add_executable(test_hdc ${SRCS})
target_link_libraries(test_hdc hdc chdc gtest gmock_main)
add_test(NAME cpp_test COMMAND test_hdc)

install(TARGETS test_hdc DESTINATION bin)



#set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}"/lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# install(TARGETS test_hdc_cpp RUNTIME DESTINATION bin)
