#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

add_library(pak_lib STATIC
    "pak_file.cxx"
)

target_compile_features(pak_lib PRIVATE cxx_std_17)

target_compile_options(pak_lib
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat -Wno-c++98-compat-pedantic>
)

target_include_directories(pak_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
