//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "application.hxx"
#include "version_info.hxx"

WARNINGS_DISABLE
#include <QFont>
#include <QStyleFactory>
#include <QPalette>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QIcon>
WARNINGS_RESTORE

namespace pak
{
    Application::Application(int argc, char* argv[]) : QApplication(argc, argv)
    {
        QApplication::setOrganizationName("Jamie Kenyon");
        QApplication::setOrganizationDomain("jamiekenyon.io");
        QApplication::setApplicationName(QString::fromLocal8Bit(resources::kProjectName));
        QApplication::setApplicationVersion(QString::fromLocal8Bit(resources::kProjectVersion));

        setTheme();
    }

    void Application::setTheme()
    {
        setStyle(QString::fromLocal8Bit("Fusion"));

        QFile f(":qdarkstyle/style.qss");

        if (f.exists())
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            setStyleSheet(ts.readAll());
        }

        auto frame = QIcon(":frame.png");
        if (!frame.isNull())
        {
            setWindowIcon(frame);
        }
    }
} // namespace pak
