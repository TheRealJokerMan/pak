//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#pragma once

// clang-format off

#if !defined __INTELLISENSE__
    #if defined (__clang__)
        #define WARNINGS_DISABLE                                    \
            _Pragma("clang diagnostic push")                        \
            _Pragma("clang diagnostic ignored \"-Weverything\"")

        #define WARNINGS_RESTORE                                    \
            _Pragma("clang diagnostic pop")
    #elif defined (_MSC_VER)
        #define WARNINGS_DISABLE    __pragma(warning(push, 0))
        #define WARNINGS_RESTORE    __pragma(warning(pop))
    #else
        #define WARNINGS_DISABLE
        #define WARNINGS_RESTORE
    #endif // #if defined __clang__
#else
    #define WARNINGS_DISABLE
    #define WARNINGS_RESTORE
#endif // #if !defined __INTELLISENSE__

// clang-format on
