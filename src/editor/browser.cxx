//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "application.hxx"
#include "browser.hxx"
#include "mainWindow.hxx"
#include <pak_lib/pak_file.hxx>

WARNINGS_DISABLE
#include <QDir>
#include <QMainWindow>
#include <QMenuBar>
#include <QMimeData>
#include <QShortcut>
#include <QTreeWidget>
#include <QUrl>
WARNINGS_RESTORE

namespace pak
{
    BrowserTree::BrowserTree(QWidget* parent) : QTreeWidget(parent)
    {
        auto const icon = QIcon::fromTheme("edit-delete", QIcon(":/images/delete.png"));
        mDeleteAction = new QAction(icon, QString::fromUtf8("&Delete"), this);
        mDeleteAction->connect(mDeleteAction, &QAction::triggered, this, &BrowserTree::onDeleteItem);
        mDeleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
        mDeleteAction->setStatusTip(QString::fromUtf8("Delete the current item in the PAK file..."));
        mDeleteAction->setEnabled(false);

        connect(this, &QTreeWidget::itemSelectionChanged, this, &BrowserTree::onSelectionChanged);

        QTreeWidget::setAcceptDrops(true);
        QTreeWidget::setDropIndicatorShown(true);
    }

    bool BrowserTree::dropMimeData(QTreeWidgetItem* parent, [[maybe_unused]] int index, QMimeData const* data, [[maybe_unused]] Qt::DropAction action)
    {
        for (QUrl const& url : data->urls())
        {
            QFileInfo const info(url.toLocalFile());
            if (info.isFile())
            {
                QTreeWidgetItem* item;
                if (parent)
                {
                    item = new QTreeWidgetItem(parent);
                    parent->setExpanded(true);
                }
                else
                {
                    item = new QTreeWidgetItem(this);
                }

                item->setText(0, info.fileName());
            }
        }

        notifyDirtyDocument();

        return true;
    }

    QStringList BrowserTree::mimeTypes() const
    {
        return QStringList() << "text/uri-list";
    }

    void BrowserTree::onDeleteItem()
    {
        if (QTreeWidget::currentItem())
        {
            delete QTreeWidget::currentItem();

            notifyDirtyDocument();
        }
    }

    void BrowserTree::onSelectionChanged()
    {
        auto selectedItems = QTreeWidget::selectedItems();
        mDeleteAction->setEnabled(!selectedItems.empty());
    }

    void BrowserTree::notifyDirtyDocument()
    {
        foreach (QWidget* w, QApplication::topLevelWidgets())
        {
            if (QMainWindow* window = qobject_cast<QMainWindow*>(w))
            {
                window->setWindowModified(true);

                auto mainWindow = reinterpret_cast<MainWindow*>(window);
                if (mainWindow)
                {
                    mainWindow->DocumentWasModified(true);
                }
            }
        }
    }

    Browser::Browser(QWidget* parent) : QDockWidget(QString::fromUtf8("Browser"), parent)
    {
        mTreeWidget = new BrowserTree(this);
        QDockWidget::setWidget(mTreeWidget);

        mTreeWidget->setHeaderHidden(true);
    }

    static QString getTooltip(PakEntry const& entry)
    {
        QString result = QString::fromStdString(entry.GetName());

        result += QString::fromUtf8(": ");
        result += QString::number(entry.GetContents().size());
        result += QString::fromUtf8(" bytes.");

        return result;
    }

    void Browser::Initialise(QString fileName)
    {
        mTreeWidget->clear();

        if (fileName.isEmpty())
        {
            return;
        }

        PakFile& pakFile = reinterpret_cast<Application*>(QApplication::instance())->GetPakFile();
        for (auto const& entry : pakFile.GetEntries())
        {
            auto const path = QDir::cleanPath(QString::fromStdString(entry.GetName()));
            QStringList tokens = path.split('/');

            QTreeWidgetItem* treeWidgetItem = nullptr;

            for (int32_t j = 0; j < mTreeWidget->topLevelItemCount(); ++j)
            {
                if (mTreeWidget->topLevelItem(j)->text(0) == tokens.at(0))
                {
                    treeWidgetItem = mTreeWidget->topLevelItem(j);
                    break;
                }
            }

            if (!treeWidgetItem)
            {
                treeWidgetItem = new QTreeWidgetItem;
                treeWidgetItem->setText(0, tokens.at(0));
                treeWidgetItem->setStatusTip(0, getTooltip(entry));
                mTreeWidget->addTopLevelItem(treeWidgetItem);
            }

            for (int32_t j = 1; j < tokens.size(); ++j)
            {
                int32_t k;
                for (k = 0; k < treeWidgetItem->childCount(); ++k)
                {
                    if (treeWidgetItem->child(k)->text(0) == tokens.at(j))
                    {
                        treeWidgetItem = treeWidgetItem->child(k);
                        break;
                    }
                }

                if (k == treeWidgetItem->childCount())
                {
                    QTreeWidgetItem* newTreeWidgetItem = new QTreeWidgetItem;
                    newTreeWidgetItem->setText(0, tokens.at(j));
                    newTreeWidgetItem->setStatusTip(0, getTooltip(entry));
                    treeWidgetItem->addChild(newTreeWidgetItem);
                    treeWidgetItem = newTreeWidgetItem;
                }
            }
        }
    }
} // namespace pak
