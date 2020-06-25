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

        /// Document was modified.
        /// \param isDirty true if the document was modified; false otherwise.
        void DocumentWasModified(bool isDirty);

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        void createCentralWidget(int32_t windowWidth, int32_t windowHeight);
        void createMenu();
        void createStatusBar();
        
        void loadFile(QString fileName);
        bool maybeSave();
        
        void onFileClose();
        void onFileNew();
        void onFileOpen();
        bool onFileSave();
        void onHelpAbout();
        
        void setupUi();

    private:
        Browser* mBrowserWidget = nullptr;
        bool mDocumentWasModified = false;
        QAction* mFileSaveAction = nullptr;
        QWidget* mViewportWidget = nullptr;
    };
} // namespace pak
