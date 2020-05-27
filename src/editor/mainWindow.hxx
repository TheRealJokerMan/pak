//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#pragma once

#include "browser.hxx"
#include "pragmas.hxx"
#include <array>

WARNINGS_DISABLE
#include <QMainWindow>
WARNINGS_RESTORE

namespace pak
{
    /// The application class.
    class MainWindow final : public QMainWindow
    {
    public:
        MainWindow();

    private:
        void createCentralWidget(int32_t windowWidth, int32_t windowHeight);
        void createMenu();
        void createStatusBar();
        
        void loadFile(QString fileName);
        void onFileOpen();
        void onHelpAbout();
        
        void setupUi();

    private:
        Browser* mBrowserWidget = nullptr;
        QWidget* mViewportWidget = nullptr;
    };
} // namespace pak
