#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

function(qt5_wrap_ui_custom targetName)
    set(_QT5_INTERNAL_SCOPE ON)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})

    foreach(it ${ui_files})
        get_filename_component(outfile ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.hxx)
        add_custom_command(OUTPUT ${outfile} COMMAND ${Qt5Widgets_UIC_EXECUTABLE} ARGS ${ui_options} -o ${outfile} ${infile} MAIN_DEPENDENCY ${infile} VERBATIM)
        target_sources(${targetName} PRIVATE ${outfile})
        target_include_directories(${targetName} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
    endforeach()
endfunction(qt5_wrap_ui_custom)

function(qt5_add_resources_custom targetName)
    set(_QT5_INTERNAL_SCOPE ON)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_RCC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
    set(rcc_options ${_RCC_OPTIONS})

    if("${rcc_options}" MATCHES "-binary")
        message(WARNING "Use qt5_add_binary_resources for binary option")
    endif()

    foreach(it ${rcc_files})
        get_filename_component(outfilename ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${outfilename}.cpp)

        _QT5_PARSE_QRC_FILE(${infile} _out_depends _rc_depends)
        set_source_files_properties(${infile} PROPERTIES SKIP_AUTORCC ON)

        add_custom_command(OUTPUT ${outfile}
                           COMMAND ${Qt5Core_RCC_EXECUTABLE}
                           ARGS ${rcc_options} --name ${outfilename} --output ${outfile} ${infile}
                           MAIN_DEPENDENCY ${infile}
                           DEPENDS ${_rc_depends} "${_out_depends}" VERBATIM)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
        set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
        target_sources(${targetName} PRIVATE ${outfile})
        set_property(SOURCE ${outfile} PROPERTY COMPILE_FLAGS $<$<CXX_COMPILER_ID:Clang>:-W0>)
    endforeach()
endfunction(qt5_add_resources_custom)

function(qt5_copy_runtime targetName)
    add_custom_command(TARGET ${targetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${targetName}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${targetName}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${targetName}>
    )

    if (CMAKE_HOST_WIN32)
        add_custom_command(TARGET ${targetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${targetName}>/platforms/$<TARGET_FILE_NAME:Qt5::QWindowsIntegrationPlugin>
        )
    endif (CMAKE_HOST_WIN32)
endfunction(qt5_copy_runtime)

function(qt5_wrap_cpp_custom targetName)
    set(_QT5_INTERNAL_SCOPE ON)
    
    # get include dirs
    qt5_get_moc_flags(moc_flags)

    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs OPTIONS DEPENDS)

    cmake_parse_arguments(_WRAP_CPP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(moc_files ${_WRAP_CPP_UNPARSED_ARGUMENTS})
    set(moc_options ${_WRAP_CPP_OPTIONS})
    set(moc_target ${_WRAP_CPP_TARGET})
    set(moc_depends ${_WRAP_CPP_DEPENDS})

    foreach(it ${moc_files})
        get_filename_component(it ${it} ABSOLUTE)
        qt5_make_output_file(${it} moc_ cpp outfile)
        qt5_create_moc_command(${it} ${outfile} "${moc_flags}" "${moc_options}" "${moc_target}" "${moc_depends}")
        
        target_sources(${targetName} PRIVATE ${outfile})
        set_property(SOURCE ${outfile} PROPERTY COMPILE_FLAGS $<$<CXX_COMPILER_ID:Clang>:-W0>)
    endforeach()
endfunction(qt5_wrap_cpp_custom)
