#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

function(embed_versioning targetName)
    configure_file("${CMAKE_SOURCE_DIR}/src/resources/frame.ico" "frame.ico" COPYONLY)
    configure_file("${CMAKE_SOURCE_DIR}/src/resources/resources.hxx" "resources.hxx" COPYONLY)
    configure_file("${CMAKE_SOURCE_DIR}/src/resources/version.rc.in" "version.rc" @ONLY)
    configure_file("${CMAKE_SOURCE_DIR}/src/resources/version_info.hxx.in" "version_info.hxx" @ONLY)

    target_include_directories(${targetName} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

    if (CMAKE_HOST_WIN32)
        target_sources(${targetName} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/version.rc")
        set_property(SOURCE "${CMAKE_CURRENT_BINARY_DIR}/version.rc" PROPERTY COMPILE_FLAGS /nologo)
    endif(CMAKE_HOST_WIN32)
endfunction(embed_versioning)
