# this is the required version of CMake necessary
# to run. CMake will fail if you have anything
# less than 3.3
cmake_minimum_required( VERSION 3.3 )

# when CMake generates Makefiles, this setting
# will allow them to print to the console.
set( CMAKE_VERBOSE_MAKEFILE "ON" )

# this points to the root of the cmake directory
# which contains all the toolchains and modules
get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cmake ABSOLUTE )

get_filename_component( CMAKE_GLOBAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../build/cmake ABSOLUTE )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )
list( APPEND CMAKE_MODULE_PATH ${CMAKE_GLOBAL_ROOT}/modules )

# Project name
project( Async CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cpp/src ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${SRC_ROOT}/../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../build/out/include ABSOLUTE )

# this is where we could get the Version numbering from Version.h and
# save them as CMake variables. for now, lets just declare them
# set( PROJECT_VERSION_MAJOR "1" )
# set( PROJECT_VERSION_MINOR "0" )
# set( PROJECT_VERSION_PATCH "0" )

# Source files
set( ASYNC_SRCS
    ${SRC_ROOT}/AddInts.cc
)

# Public header files (installed together with libraries)
set( ASYNC_PUBLIC_HEADERS
    ${INC_ROOT}/Async/LibraryExport.h
    ${INC_ROOT}/Async/AddInts.h
)

set( ASYNC_PRIVATE_HEADERS
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${ASYNC_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
    ${CONFIG_ROOT}
)

set( ASYNC_COMPILE_DEFINITIONS )

# check required libraries and header files and such.
# this will fail CMake if set to False due to required
# modules requiring their own dependencies
set( DEPENDENCY_CHECK FALSE )

include( GNUInstallDirs )

set( ${PROJECT_NAME}_IMPORTED_LIST
    # all libraries to import when building Async
    "Utilities"
)

# Logging library
set( ASYNC_LIB_NAME ${PROJECT_NAME} )

include(rbuild)

rbuild_add_shared_library( ${PROJECT_NAME}
    "${ASYNC_SRCS}"
    "${ASYNC_PUBLIC_HEADERS}"
    "${ASYNC_PRIVATE_HEADERS}"
    "${ASYNC_COMPILE_DEFINITIONS}"
    "${${PROJECT_NAME}_IMPORTED_LIST}"
    "${INCLUDE_DIRS}"
)

if( RUN_UNIT_TESTS )

    get_filename_component( TEST_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cpp/unitTest/src ABSOLUTE )
    get_filename_component( TEST_INC_ROOT ${TEST_SRC_ROOT}/../include ABSOLUTE )
    
    # list( APPEND CMAKE_FIND_ROOT_PATH ${INSTALL_ROOT} )

    set( ${PROJECT_NAME}_unit_IMPORTED_LIST
        # all libraries to import when building Async
        "gtest"
        "Utilities"
    )

    # source files
    set( ASYNC_UNIT_SOURCES
        ${TEST_SRC_ROOT}/TestAddInts.cc
        ${TEST_SRC_ROOT}/main.cc
    )

    set( ASYNC_UNIT_HEADERS
    )

    # directories to search for header files
    set( INCLUDE_DIRS
        ${INC_ROOT}
        ${TEST_INC_ROOT}
        ${CONFIG_ROOT}
    )
    
    rbuild_add_gtest( ${PROJECT_NAME}_unit
        "${ASYNC_UNIT_SOURCES}"
        "${ASYNC_UNIT_HEADERS}"
        "${${PROJECT_NAME}_unit_IMPORTED_LIST}"
        "${INCLUDE_DIRS}"
    )
    if( TARGET ${PROJECT_NAME}_unit )
        target_link_libraries( ${PROJECT_NAME}_unit ${PROJECT_NAME} )
    endif()
endif()