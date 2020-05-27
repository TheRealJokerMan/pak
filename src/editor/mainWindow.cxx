//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "aboutDialog.hxx"
#include "application.hxx"
#include "mainWindow.hxx"

WARNINGS_DISABLE
#include <QDockWidget>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenuBar>
#include <QScreen>
#include <QSettings>
#include <QStatusBar>
#include <QTextEdit>
#include <QTreeWidget>
WARNINGS_RESTORE

namespace pak
{
    MainWindow::MainWindow()
    {
        setupUi();
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
        QAction* action = nullptr;
        QMenu* menu = nullptr;

        menu = QMainWindow::menuBar()->addMenu(QString::fromLocal8Bit("&File"));

        action = menu->addAction(QString::fromLocal8Bit("&Open"), this, &MainWindow::onFileOpen);
        action->setShortcuts(QKeySequence::Open);
        action->setStatusTip(tr("Open a PAK file..."));

        menu->addSeparator();
        
        action = menu->addAction(QString::fromLocal8Bit("&Quit"), this, &QWidget::close);
        action->setShortcuts(QKeySequence::Quit);
        action->setStatusTip(tr("Quit the application..."));

        menu = QMainWindow::menuBar()->addMenu(QString::fromLocal8Bit("&Help"));
        action = menu->addAction(QString::fromLocal8Bit("&About"), this, &MainWindow::onHelpAbout);
        action->setStatusTip(tr("Show information about this application..."));
    }

    void MainWindow::createStatusBar()
    {
        QMainWindow::statusBar()->showMessage(QString::fromLocal8Bit("Ready"));
    }

    void MainWindow::loadFile(QString fileName)
    {
        auto app = reinterpret_cast<Application*>(QApplication::instance());
        if (app->GetPakFile().Read(fileName.toStdString()))
        {
            mBrowserWidget->Initialise(fileName);

            QSettings settings;
            settings.setValue(QString::fromLocal8Bit("lastDir"), QFileInfo(fileName).absoluteDir().absolutePath());
            
            QMainWindow::setWindowFilePath(fileName);
            statusBar()->showMessage(QString::fromLocal8Bit("File loaded"), 2000);
        }
        else
        {
            QMainWindow::statusBar()->showMessage(QString::fromLocal8Bit("Failed to load PAK file!"));
        }
    }

    void MainWindow::onFileOpen()
    {
        QSettings settings;
        auto dir = settings.value(QString::fromLocal8Bit("lastDir")).toString();

        QString fileName = QFileDialog::getOpenFileName(this, ("Open File"), dir, ("PAK files (*.pak)"));
        if (!(fileName.isNull() && fileName.isEmpty()))
        {
            loadFile(fileName);
        }
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

        createMenu();
        createStatusBar();

        createCentralWidget(windowWidth, windowHeight);
    }
} // namespace pak
