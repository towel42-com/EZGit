function( DeploySystem target directory)
    set( options )
    set( oneValueArgs INSTALL_ONLY )
    set( multiValueArgs )

    cmake_parse_arguments( "" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    
    #message( STATUS "Deploy System ${target}" )
    if ( WIN32 )
        set(CMAKE_INSTALL_UCRT_LIBRARIES FALSE)
        #set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE ) 
    ENDIF()

    # deployqt doesn't work correctly with the system runtime libraries,
    # so we fall back to one of CMake's own modules for copying them over
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
    include(InstallRequiredSystemLibraries)

    if ( NOT _INSTALL_ONLY )
        #message( STATUS "${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}" )
        foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
            get_filename_component(filename "${lib}" NAME)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E echo "Deploying System Library '${filename}' for '${target}'"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${lib}" \"$<TARGET_FILE_DIR:${target}>\"
            )
        endforeach()
    endif()
endfunction()

