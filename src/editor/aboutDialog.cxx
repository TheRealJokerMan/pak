//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "aboutDialog.hxx"

namespace pak
{
    AboutDialog::AboutDialog()
    {
        setupUi(this);

        mProductName->setText(QApplication::instance()->applicationName());
        mProductVersion->setText(QApplication::instance()->applicationVersion());
    }

    void AboutDialog::Show()
    {
        exec();
    }
} // namespace pak
