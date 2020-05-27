//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "application.hxx"
#include "browser.hxx"
#include <pak_lib/pak_file.hxx>

WARNINGS_DISABLE
#include <QDir>
#include <QMimeData>
#include <QShortcut>
#include <QTreeWidget>
#include <QUrl>
WARNINGS_RESTORE

namespace pak
{
    BrowserTree::BrowserTree(QWidget* parent) : QTreeWidget(parent)
    {
        QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
        connect(shortcut, &QShortcut::activated, this, &BrowserTree::onDeleteItem);

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
        }
    }

    Browser::Browser(QWidget* parent) : QDockWidget(QString::fromLocal8Bit("Browser"), parent)
    {
    }

    void Browser::Initialise([[maybe_unused]] QString fileName)
    {
        if (!mTreeWidget)
        {
            mTreeWidget = new BrowserTree(this);
            QDockWidget::setWidget(mTreeWidget);
        }

        mTreeWidget->clear();
        mTreeWidget->setHeaderHidden(true);

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
                    treeWidgetItem->addChild(newTreeWidgetItem);
                    treeWidgetItem = newTreeWidgetItem;
                }
            }
        }
    }
} // namespace pak
