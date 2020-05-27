//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#pragma once

#include "pragmas.hxx"

WARNINGS_DISABLE
#include <QDockWidget>
#include <QTreeWidget>
WARNINGS_RESTORE

namespace pak
{
    class BrowserTree final : public QTreeWidget
    {
    public:
        /// Constructor.
        /// \param parent The parent of this widget.
        BrowserTree(QWidget* parent);

    protected:
        bool dropMimeData(QTreeWidgetItem* parent, int index, QMimeData const* data, Qt::DropAction action) override;
        QStringList mimeTypes() const override;
    
    private:
        void onDeleteItem();
    };
    
    /// A Browser widget.
    class Browser final : public QDockWidget
    {
    public:
        /// Constructor.
        /// \param parent The parent of this widget.
        Browser(QWidget* parent);

        /// Initialise the widget.
        /// \param fileName Path of the PAK file.
        void Initialise(QString fileName);

    private:
        BrowserTree* mTreeWidget = nullptr;
    };
} // namespace pak
