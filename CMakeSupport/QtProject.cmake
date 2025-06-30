cmake_minimum_required(VERSION 3.22)

include( ${CMAKE_CURRENT_LIST_DIR}/Project.cmake )

find_package(Qt6 COMPONENTS Core Widgets  REQUIRED)
if ( NOT QTDIR )
    set(QTDIR ${_qt6Core_install_prefix} CACHE BOOL "Has QTDIR been reported" )
endif()

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_AUTORCC OFF)
SET(CMAKE_AUTOUIC OFF)

UNSET( qtproject_UIS_H )
UNSET( qtproject_MOC_SRCS )
UNSET( qtproject_CPPMOC_H )
UNSET( qtproject_QRC_SRCS )
QT6_WRAP_CPP(qtproject_MOC_SRCS ${qtproject_H})
QT6_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})
QT6_ADD_RESOURCES( qtproject_QRC_SRCS ${qtproject_QRC} )

source_group("Generated Files" FILES ${qtproject_UIS_H} ${qtproject_MOC_SRCS} ${qtproject_QRC_SRCS} ${qtproject_CPPMOC_H})
source_group("Resource Files"  FILES ${qtproject_QRC} ${qtproject_QRC_SOURCES} )
source_group("Designer Files"  FILES ${qtproject_UIS} )
source_group("Header Files"    FILES ${qtproject_H} )

source_group("Source Files"    FILES ${qtproject_CPPMOC_SRCS} )
source_group("Source Files"    FILES ${qtproject_SRCS} )


SET( _PROJECT_DEPENDENCIES
	${_PROJECT_DEPENDENCIES}
    ${qtproject_SRCS} 
    ${qtproject_QRC} 
    ${qtproject_QRC_SRCS} 
    ${qtproject_UIS_H} 
    ${qtproject_MOC_SRCS} 
    ${qtproject_H} 
    ${qtproject_UIS}
    ${qtproject_QRC_SOURCES}
)

SET( project_pub_DEPS
     Qt6::Core
     ${project_pub_DEPS}
     )

SET( project_pri_DEPS
    # insert and "global default" qt private depends here
    ${project_pri_DEPS}
)
