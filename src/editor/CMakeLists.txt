#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

include(Qt5Helpers)
include(Versioning)

add_executable(pak_editor WIN32
    "aboutDialog.cxx"
    "application.cxx"
    "browser.cxx"
    "main.cxx"
    "mainWindow.cxx"
 )

 embed_versioning(pak_editor)

 qt5_add_resources_custom(pak_editor
    "qresources/qdarkstyle/style.qrc"
)

qt5_wrap_ui_custom(pak_editor
    "qresources/ui/about.ui"
)

target_compile_features(pak_editor PRIVATE cxx_std_17)

target_compile_options(pak_editor
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat -Wno-c++98-compat-pedantic>
)

target_link_libraries(pak_editor
    pak_lib
    Qt5::Widgets
)

qt5_copy_runtime(pak_editor)
