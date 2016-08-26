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
    ${SRC_ROOT}/Enums/ConcurrencyStates.cc
    ${SRC_ROOT}/Detail/Semaphore.cc

    ${SRC_ROOT}/QueueableObject.cc
    ${SRC_ROOT}/ExternalRefCount.cc
    ${SRC_ROOT}/WorkerThread.cc
    ${SRC_ROOT}/GarbageCollector.cc
    ${SRC_ROOT}/WorkObject.cc
    ${SRC_ROOT}/Engine.cc
    ${SRC_ROOT}/Async.cc
)

# Public header files (installed together with libraries)
set( ASYNC_PUBLIC_HEADERS
    ${INC_ROOT}/Async/LibraryExport.h
    ${INC_ROOT}/Async/Enums/ConcurrencyStates.h

    # ${INC_ROOT}/Async/Interfaces/IExecutableWorkItem.h
    ${INC_ROOT}/Async/Interfaces/IRefCountedObject.h
    ${INC_ROOT}/Async/QueueableObject.h
    ${INC_ROOT}/Async/Interfaces/IThread.h
    ${INC_ROOT}/Async/Interfaces/IGarbageCollector.h
    ${INC_ROOT}/Async/Interfaces/IContinuableObject.h
    ${INC_ROOT}/Async/Interfaces/IValueHolder.h

    ${INC_ROOT}/Async/Async.h
)

set( ASYNC_PRIVATE_HEADERS
    ${INC_ROOT}/Async/Detail/Semaphore.h
    ${INC_ROOT}/Async/WorkerThread.h
    ${INC_ROOT}/Async/GarbageCollector.h
    ${INC_ROOT}/Async/WorkObject.h
    ${INC_ROOT}/Async/Engine.h
    ${INC_ROOT}/Async/ExternalRefCount.h
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${ASYNC_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
    ${CONFIG_ROOT}
)

include_directories( ${INCLUDE_DIRS} )

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
        ${SRC_ROOT}/Enums/ConcurrencyStates.cc
        ${SRC_ROOT}/Detail/Semaphore.cc
        ${SRC_ROOT}/QueueableObject.cc
        ${SRC_ROOT}/WorkerThread.cc
        ${SRC_ROOT}/GarbageCollector.cc
        ${SRC_ROOT}/WorkObject.cc
        ${SRC_ROOT}/Engine.cc

        ${TEST_SRC_ROOT}/QueueableObjectTestChild.cc
        ${TEST_SRC_ROOT}/DynamicQueueableObjectTestChild.cc
        ${TEST_SRC_ROOT}/ValueHolderTestChild.cc
        ${TEST_SRC_ROOT}/GarbageCollector_noDealloc.cc
        ${TEST_SRC_ROOT}/SharedMemory.cc
        ${TEST_SRC_ROOT}/UnitTest_Utils.cc

        ${TEST_SRC_ROOT}/WorkerThread_Solo_unit.cc
        ${TEST_SRC_ROOT}/GarbageCollector_Solo_unit.cc
        ${TEST_SRC_ROOT}/QueueableObject_Solo_unit.cc
        ${TEST_SRC_ROOT}/QueueableObject_WorkerThread_unit.cc
        ${TEST_SRC_ROOT}/QueueableObject_WorkerThread_GarbageCollector_unit.cc
        ${TEST_SRC_ROOT}/ValueHolderTestChild_unit.cc
        ${TEST_SRC_ROOT}/WorkObject_Solo_unit.cc
        ${TEST_SRC_ROOT}/WorkObject_ValueHolderTestChild_WorkerThread_unit.cc
        ${TEST_SRC_ROOT}/WorkObject_ValueHolderTestChild_WorkerThread_GarbageCollector_unit.cc
        ${TEST_SRC_ROOT}/Engine_unit.cc
        ${TEST_SRC_ROOT}/Async_unit.cc
        ${TEST_SRC_ROOT}/main.cc
    )

    set( ASYNC_UNIT_HEADERS
        ${TEST_INC_ROOT}/QueueableObjectTestChild.h
        ${TEST_INC_ROOT}/DynamicQueueableObjectTestChild.h
        ${TEST_INC_ROOT}/ValueHolderTestChild.h
        ${TEST_INC_ROOT}/GarbageCollector_noDealloc.h
        ${TEST_INC_ROOT}/SharedMemory.h
        ${TEST_INC_ROOT}/UnitTest_Utils.h
    )

    # directories to search for header files
    set( INCLUDE_DIRS
        ${INC_ROOT}
        ${TEST_INC_ROOT}
        ${CONFIG_ROOT}
    )

    include_directories( ${INCLUDE_DIRS} )
    rbuild_add_gtest( ${PROJECT_NAME}_unit
        "${ASYNC_UNIT_SOURCES}"
        "${ASYNC_UNIT_HEADERS}"
        "${${PROJECT_NAME}_unit_IMPORTED_LIST}"
    )
    if( TARGET ${PROJECT_NAME}_unit )
        target_link_libraries( ${PROJECT_NAME}_unit ${PROJECT_NAME} )
    endif()
endif()
