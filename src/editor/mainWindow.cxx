//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "aboutDialog.hxx"
#include "application.hxx"
#include "mainWindow.hxx"

WARNINGS_DISABLE
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QTreeWidget>
WARNINGS_RESTORE

namespace pak
{
    MainWindow::MainWindow()
    {
        setupUi();
    }

    void MainWindow::DocumentWasModified(bool isDirty)
    {
        mDocumentWasModified = isDirty;
        mFileSaveAction->setEnabled(isDirty);
    }

    void MainWindow::closeEvent(QCloseEvent* event)
    {
        if (maybeSave())
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }

    void MainWindow::createCentralWidget(int32_t windowWidth, int32_t windowHeight)
    {
        mViewportWidget = new QWidget(this);
        mViewportWidget->setMaximumSize(static_cast<int>(windowWidth * 0.8), static_cast<int>(windowHeight * 0.6));
        QMainWindow::setCentralWidget(mViewportWidget);

        mBrowserWidget = new Browser(this);
        QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, mBrowserWidget);
    }

    void MainWindow::createMenu()
    {
        QToolBar* toolBar = addToolBar(QString::fromUtf8("Tools"));

        // File menu.
        auto fileMenu = QMainWindow::menuBar()->addMenu(QString::fromUtf8("&File"));

        auto fileNewIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
        auto fileNewAction = fileMenu->addAction(fileNewIcon, QString::fromUtf8("&New"), this, &MainWindow::onFileNew, QKeySequence::New);
        fileNewAction->setStatusTip(QString::fromUtf8("Create a PAK file..."));
        toolBar->addAction(fileNewAction);

        auto fileOpenIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
        auto fileOpenAction = fileMenu->addAction(fileOpenIcon, QString::fromUtf8("&Open"), this, &MainWindow::onFileOpen, QKeySequence::Open);
        fileOpenAction->setStatusTip(tr("Open a PAK file..."));
        toolBar->addAction(fileOpenAction);

        auto fileSaveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
        mFileSaveAction = fileMenu->addAction(fileSaveIcon, QString::fromUtf8("&Save"), this, &MainWindow::onFileSave, QKeySequence::Save);
        mFileSaveAction->setStatusTip(QString::fromUtf8("Save the PAK file..."));
        mFileSaveAction->setEnabled(false);
        toolBar->addAction(mFileSaveAction);

        fileMenu->addSeparator();

        auto fileCloseAction = fileMenu->addAction(QString::fromUtf8("&Close"), this, &MainWindow::onFileClose);
        fileCloseAction->setStatusTip(tr("Close the current PAK file..."));

        fileMenu->addSeparator();

        auto fileQuitAction = fileMenu->addAction(QString::fromUtf8("&Quit"), this, &QWidget::close, QKeySequence::Quit);
        fileQuitAction->setStatusTip(tr("Quit the application..."));

        // Edit menu.
        auto editMenu = QMainWindow::menuBar()->addMenu(QString::fromUtf8("&Edit"));

        editMenu->addAction(mBrowserWidget->GetDeleteAction());
        toolBar->addAction(mBrowserWidget->GetDeleteAction());

        // Help menu.
        auto helpMenu = QMainWindow::menuBar()->addMenu(QString::fromUtf8("&Help"));
        auto helpAboutAction = helpMenu->addAction(QString::fromUtf8("&About"), this, &MainWindow::onHelpAbout);
        helpAboutAction->setStatusTip(tr("Show information about this application..."));
    }

    void MainWindow::createStatusBar()
    {
        QMainWindow::statusBar()->showMessage(QString::fromUtf8("Ready"));
    }

    void MainWindow::loadFile(QString fileName)
    {
        mDocumentWasModified = false;
        QMainWindow::setWindowModified(false);
        QMainWindow::setWindowFilePath(fileName);

        if (!fileName.isEmpty())
        {
            auto app = reinterpret_cast<Application*>(QApplication::instance());
            if (app->GetPakFile().Read(fileName.toStdString()))
            {
                QSettings settings;
                settings.setValue(QString::fromUtf8("lastDir"), QFileInfo(fileName).absoluteDir().absolutePath());

                statusBar()->showMessage(QString::fromUtf8("File loaded"), 2000);
            }
            else
            {
                QMainWindow::statusBar()->showMessage(QString::fromUtf8("Failed to load PAK file!"));
            }
        }

        mBrowserWidget->Initialise(fileName);
    }

    bool MainWindow::maybeSave()
    {
        if (!mDocumentWasModified)
        {
            return true;
        }

        auto ret = QMessageBox::warning(this, QApplication::instance()->applicationName(), QString::fromUtf8("Do you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            return onFileSave();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }

        return true;
    }

    void MainWindow::onFileClose()
    {
        if (maybeSave())
        {
            loadFile(QString());
        }
    }

    void MainWindow::onFileNew()
    {
        if (maybeSave())
        {
            loadFile(QString());
        }
    }

    void MainWindow::onFileOpen()
    {
        QSettings settings;
        auto dir = settings.value(QString::fromUtf8("lastDir")).toString();

        QString fileName = QFileDialog::getOpenFileName(this, ("Open File"), dir, ("PAK files (*.pak)"));
        if (!(fileName.isNull() && fileName.isEmpty()))
        {
            loadFile(fileName);
        }
    }

    bool MainWindow::onFileSave()
    {
        mDocumentWasModified = false;
        QMainWindow::setWindowModified(false);
        return true;
    }

    void MainWindow::onHelpAbout()
    {
        auto dialog = AboutDialog();
        dialog.Show();
    }

    void MainWindow::setupUi()
    {
        auto const screenGeometry = QGuiApplication::primaryScreen()->geometry();
        auto const screenWidth = screenGeometry.width();
        auto const screenHeight = screenGeometry.height();

        auto const windowWidth = static_cast<int>(screenWidth * 0.8);
        auto const windowHeight = static_cast<int>(screenHeight * 0.8);

        QMainWindow::resize(windowWidth, windowHeight);
        QMainWindow::move((screenWidth - windowWidth) >> 1, (screenHeight - windowHeight) >> 1);

        createCentralWidget(windowWidth, windowHeight);
        createMenu();
        createStatusBar();
    }
} // namespace pak
