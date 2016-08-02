find_path( ASYNC_INCLUDES Async/LibraryExport.h
    HINTS
        ${CMAKE_INSTALL_PREFIX}/include
        ${CMAKE_PREFIX_PATH}/include
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( ASYNC_SHARED_LIB Async.dll
        HINTS
            ${CMAKE_INSTALL_PREFIX}/bin
            ${CMAKE_PREFIX_PATH}/bin
    )
    find_library( ASYNC_LIB_IMPL Async
        HINTS
            ${CMAKE_INSTALL_PREFIX}/lib
            ${CMAKE_PREFIX_PATH}/lib
    )
else()
    find_library( ASYNC_SHARED_LIB Async
        HINTS
            ${CMAKE_INSTALL_PREFIX}/lib
            ${CMAKE_PREFIX_PATH}/lib
    )
endif()

# make library target
if( NOT TARGET ASYNC_LIB )
    add_library( ASYNC_LIB SHARED IMPORTED )

    # will set .so for unix systems and .dll for windows
    set_property( TARGET ASYNC_LIB PROPERTY
        IMPORTED_LOCATION ${ASYNC_SHARED_LIB} )

    # need to link to .lib files for windows
    if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
        set_property( TARGET ASYNC_LIB PROPERTY
            IMPORTED_IMPLIB ${ASYNC_LIB_IMPL} )
    endif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
endif()

if( ASYNC_INCLUDES AND ASYNC_LIB )
    set( ASYNC_FOUND TRUE )
else()
    # set( ASYNC_SHARED_LIB ${ASYNC_SHARED_LIB}/Async.dll )
    set( ASYNC_FOUND FALSE )
endif()
