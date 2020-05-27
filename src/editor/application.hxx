//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#pragma once

#include "pragmas.hxx"
#include <pak_lib/pak_file.hxx>

WARNINGS_DISABLE
#include <QApplication>
WARNINGS_RESTORE

namespace pak
{
    /// The application class.
    class Application final : public QApplication
    {
    public:
        /// Constructor.
        /// \param argc The number of arguments supplied to the application.
        /// \param argv The arguments supplied to the application.
        Application(int argc, char* argv[]);

        /// PAK file accessor.
        /// \return The PAK file.
        PakFile& GetPakFile()
        {
            return mPakFile;
        }

    private:
        void setTheme();

    private:
        PakFile mPakFile;
    };
} // namespace pak
