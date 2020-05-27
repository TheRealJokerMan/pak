#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

include(Versioning)

add_executable(pak_make
    "main.cxx"
)

embed_versioning(pak_make)

target_compile_features(pak_make PRIVATE cxx_std_17)

target_compile_options(pak_make
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat -Wno-c++98-compat-pedantic>
)

target_link_libraries(pak_make
    pak_lib
)
