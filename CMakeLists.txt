cmake_minimum_required(VERSION 3.22)

project(root-EZGit
    VERSION 1.0 
    DESCRIPTION "Tool for a basic git wizard"
    HOMEPAGE_URL http://towel42.com
    ) 

find_package(Qt6 COMPONENTS Core Widgets Network SerialPort Bluetooth REQUIRED)

SET( T42_GLOBAL_USE_QT true )
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/T42-CMakeUtils/Modules ${CMAKE_MODULE_PATH})

include( ${CMAKE_SOURCE_DIR}/VersionInfo.cmake )

SET( PORTABLE_INSTALL ON CACHE BOOL "Create a portable installation  parallel to the build directory" )
if ( PORTABLE_INSTALL )
    if ( NOT EXISTS ${CMAKE_SOURCE_DIR}/install )
        file( MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/install )
    endif()
    file( REAL_PATH ${CMAKE_SOURCE_DIR}/install CMAKE_INSTALL_PREFIX EXPAND_TILDE)
endif()

set( CMAKE_INSTALL_BINDIR . )
#message( STATUS "LOCAL INSTALL DIR=${CMAKE_INSTALL_PREFIX}" )

cmake_policy(SET CMP0144 NEW)
cmake_policy(SET CMP0139 NEW)

set(T42_CXX_STANDARD 20)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

add_subdirectory( T42-CMakeUtils )
add_subdirectory( gui )
add_subdirectory( Ui )

include( InstallerInfo.cmake )

