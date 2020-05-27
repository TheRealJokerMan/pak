//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#pragma once

#include "pragmas.hxx"

WARNINGS_DISABLE
#include "ui_about.hxx"
WARNINGS_RESTORE

namespace pak
{
    /// The application's frame (main) window.
    class AboutDialog final : public QDialog, public Ui_AboutDialog
    {
    public:
        /// Constructor.
        AboutDialog();

        /// Show the dialog.
        void Show();
    };
} // namespace pak
