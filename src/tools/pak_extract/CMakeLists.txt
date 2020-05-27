#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

include(Versioning)

add_executable(pak_extract
    "main.cxx"
)

embed_versioning(pak_extract)

target_compile_features(pak_extract PRIVATE cxx_std_17)

target_compile_options(pak_extract
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat -Wno-c++98-compat-pedantic>
)

target_link_libraries(pak_extract
    pak_lib
)
