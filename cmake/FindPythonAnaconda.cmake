# tested on OSX Yosemite and Ubuntu 14.04 LTS
# handle anaconda dependencies
cmake_minimum_required(VERSION 3.7)

option(ANACONDA_PYTHON_VERBOSE "Anaconda dependency info" OFF)

if(NOT CMAKE_FIND_ANACONDA_PYTHON_INCLUDED)
  set(CMAKE_FIND_ANACONDA_PYTHON_INCLUDED 1)

  # find anaconda installation
  set(_cmd conda info --root)
  execute_process(
    COMMAND ${_cmd}
    RESULT_VARIABLE _r
    OUTPUT_VARIABLE _o
    ERROR_VARIABLE _e
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
    )
  
  if(ANACONDA_PYTHON_VERBOSE)
    message("Executing conda info --root")
    message("_r = ${_r}")
    message("_o = ${_o}")
    message("_e = ${_e}")
  endif()

  IF(IS_DIRECTORY ${_o})
     set(ANACONDA_PYTHON_FOUND True)
  endif()

  if(ANACONDA_PYTHON_FOUND)
    set( ANACONDA_PYTHON_DIR ${_o} )
    message( "Found anaconda root directory ${ANACONDA_PYTHON_DIR}" )

    # find python version
    #
    set(_cmd python --version)
    execute_process(
      COMMAND ${_cmd}
      RESULT_VARIABLE _r
      OUTPUT_VARIABLE _o
      ERROR_VARIABLE _e
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE
      )
    
    if(ANACONDA_PYTHON_VERBOSE)
      message("Executing python --version")
      message("_r = ${_r}")
      message("_o = ${_o}")
      message("_e = ${_e}")
    endif()
    
    string (REGEX MATCH "Python ([0-9]+)[.]([0-9]+)[.]([0-9]+)" _py_version_found "${_o}")
    #message("_py_version_found = ${_py_version_found}")
    #message("CMAKE_MATCH_0 = ${CMAKE_MATCH_0}")
    set( _py_version_major ${CMAKE_MATCH_1} )
    set( _py_version_minor ${CMAKE_MATCH_2} )
    set( _py_version_patch ${CMAKE_MATCH_3} )
    set( ANACONDA_PYTHON_VERSION ${_py_version_major}.${_py_version_minor} )

    
    if( ${_py_version_major} MATCHES 2 )
      set( _py_ext "")
    else()
      set( _py_ext "m")
    endif()

    set(_py_id "python${ANACONDA_PYTHON_VERSION}${_py_ext}")
    
    if( NOT DEFINED ENV{CONDA_DEFAULT_ENV} )
      set( env_CONDA_DEFAULT_ENV "root" )
      # message( WARNING "Could not find anaconda environment setting; using default root" )
    else()
      set( env_CONDA_DEFAULT_ENV $ENV{CONDA_DEFAULT_ENV} )
    endif()

    message( "Using anaconda ${env_CONDA_DEFAULT_ENV} environment" )

    # find include path and Python dynamic library
    exec_program("python -c \"import sysconfig; print\(sysconfig.get_path\('include'\)\)\"" OUTPUT_VARIABLE PYTHON_INCLUDE_DIR_)
    exec_program("python -c \"from sysconfig import get_config_var; print\(\\\"%s/%s\\\" % \(get_config_var\(\\\"LIBDIR\\\"\), get_config_var\(\\\"INSTSONAME\\\"\)\)\)\"" OUTPUT_VARIABLE PYTHON_LIBRARY_)

    set( PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_DIR_}" CACHE INTERNAL "")
    set( PYTHON_LIBRARY "${PYTHON_LIBRARY_}" CACHE INTERNAL "")

    message( "PYTHON_INCLUDE_DIR = ${PYTHON_INCLUDE_DIR}")
    message( "PYTHON_LIBRARY = ${PYTHON_LIBRARY}")

    set(FOUND_PYTHONLIBS TRUE)

  else()
    message( "Not found: anaconda root directory..." )    
    message( "Trying system python install..." )    
  endif()
  
endif()

