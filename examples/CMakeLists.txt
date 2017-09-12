set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_language (Fortran)

set(CMAKE_Fortran_MODULE_DIRECTORY "${hdc_new_BINARY_DIR}/mod")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_EXAMPLES "Build and install examples" ON)
if (BUILD_EXAMPLES)

    add_library(hdc_fortran_module SHARED hdc_fortran_module.f90)
    target_link_libraries(hdc_fortran_module fhdc)

    add_executable(example_cpp example_cpp.cpp)
    target_link_libraries(example_cpp hdc)

    add_executable(example_c example_c.c)
    target_link_libraries(example_c chdc)

    add_executable(example_fortran example_fortran.f90)
    target_link_libraries(example_fortran fhdc)

    add_executable(cpo_hdc cpo_hdc.f90)
    target_link_libraries(cpo_hdc fhdc)
    
    add_executable(example_lukas example_lukas.f90)
    target_link_libraries(example_lukas fhdc)
    
    add_executable(example_interprocess example_interprocess.cpp)
    target_link_libraries(example_interprocess hdc rt)

    #add_executable(calling_fortran_from_c calling_fortran_from_c.c)
    #target_link_libraries(calling_fortran_from_c fhdc)

#     #set(CMAKE_INSTALL_RPATH "/usr/local/lib")
#     #set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# 
    install(TARGETS hdc_fortran_module  example_cpp example_c example_fortran cpo_hdc  
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION share/examples)
    
endif(BUILD_EXAMPLES)

