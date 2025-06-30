cmake_minimum_required(VERSION 3.22)

project(root-EZGit
    VERSION 1.0 
    DESCRIPTION "Tool for a basic git wizard"
    HOMEPAGE_URL http://towel42.com
    ) 

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

find_package(Qt6 COMPONENTS Core Widgets Network SerialPort Bluetooth REQUIRED)

set( QT_VERSION ${__qt_Core_version})
set( QT_INSTALL_DIR  ${QT6_INSTALL_PREFIX} )
SET( QT_BINDIR ${QT_INSTALL_DIR}/bin )
#dump_cmake_variables(".*[Qq][Tt]6.*")

WHILE( NOT EXISTS ${QT_BINDIR} )
    cmake_path( GET QT_INSTALL_DIR PARENT_PATH QT_INSTALL_DIR )
    message( STATUS "QT_INSTALL_DIR=${QT_INSTALL_DIR}" )
    cmake_path( GET QT_INSTALL_DIR PARENT_PATH __TMP )
    cmake_path( COMPARE ${QT_INSTALL_DIR} EQUAL ${QT_INSTALL_DIR} __ATROOT )
    if( __ATROOT )
        message( FATAL_ERROR "Could not determine Qt's bin dir" )
        break()
    endif()
    SET( QT_BINDIR ${QT_INSTALL_DIR}/bin )
endwhile()

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeSupport ${CMAKE_SOURCE_DIR}/qhexedit2 ${CMAKE_MODULE_PATH})

include( ${CMAKE_SOURCE_DIR}/VersionInfo.cmake )

SET( PORTABLE_INSTALL ON CACHE BOOL "Create a portable installation  parallel to the build directory" )
if ( PORTABLE_INSTALL )
    if ( NOT EXISTS ${CMAKE_SOURCE_DIR}/install )
        file( MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/install )
    endif()
    file( REAL_PATH ${CMAKE_SOURCE_DIR}/install CMAKE_INSTALL_PREFIX EXPAND_TILDE)
endif()

message( STATUS "QT_VERSION=${QT_VERSION}" )
#message( STATUS "QT_INSTALL_DIR=${QT_INSTALL_DIR}" )
#message( STATUS "QT_BINDIR=${QT_BINDIR}" )
set( CMAKE_INSTALL_BINDIR . )
message( STATUS "LOCAL INSTALL DIR=${CMAKE_INSTALL_PREFIX}" )

cmake_policy(SET CMP0144 NEW)
cmake_policy(SET CMP0139 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

add_subdirectory( CMakeSupport )
add_subdirectory( gui )
add_subdirectory( Ui )

include( InstallerInfo.cmake )

