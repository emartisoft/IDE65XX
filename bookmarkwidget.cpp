#include "bookmarkwidget.h"

BookmarkWidget::BookmarkWidget(QWidget *parent) :
    QListView(parent)
{
    delegate = new Delegate(this);
    delegate->setContentsMargins(4, 2, 4, 2);
    delegate->setIconSize(24, 24);
    delegate->setHorizontalSpacing(8);
    delegate->setVerticalSpacing(2);

    setModel(new QStandardItemModel(this));
    setItemDelegate(delegate);

    removeAction = new QAction("Remove", this);
    removeAllAction = new QAction("Remove All", this);
    editAction = new QAction("Edit", this);

    setWordWrap(true);
}

quint64 BookmarkWidget::getLineNumber(int index)
{
    QStringList topTextList = (static_cast<QStandardItemModel *>(model())->item(index)->text()).split(" : ");
    return topTextList[0].toULongLong();
}

QString BookmarkWidget::getFilepath(int index)
{
    QStringList topTextList = (static_cast<QStandardItemModel *>(model())->item(index)->text()).split(" : ");
    return topTextList[1];
}

QString BookmarkWidget::getItemText(int index)
{
    return static_cast<QStandardItemModel *>(model())->item(index)->data(Qt::UserRole).toString();
}

QIcon BookmarkWidget::getIcon(int index)
{
    return static_cast<QStandardItemModel *>(model())->item(index)->icon();
}

void BookmarkWidget::setTopText(int index, QString topText)
{
    static_cast<QStandardItemModel *>(model())->item(index)->setText(topText);
}

void BookmarkWidget::addItem(const QPixmap &pixmap, const QString &text, const QString &topText)
{
    auto *item = new QStandardItem(QIcon(pixmap), topText);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(text, Qt::UserRole);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
    scrollToBottom();
}

void BookmarkWidget::removeAll()
{
    static_cast<QStandardItemModel *>(model())->clear();
}

void BookmarkWidget::removeItem(int index)
{
    static_cast<QStandardItemModel *>(model())->removeRow(index);
}

void BookmarkWidget::setItemText(int index, QString text)
{
    static_cast<QStandardItemModel *>(model())->item(index)->setData(text, Qt::UserRole);
}

void BookmarkWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMouseEvent leftClick(QEvent::MouseButtonPress, e->pos(), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mousePressEvent(&leftClick);
    contextMenu = new QMenu;
    contextMenu->addAction(editAction);
    contextMenu->addSeparator();
    contextMenu->addAction(removeAction);
    contextMenu->addSeparator();
    contextMenu->addAction(removeAllAction);

    if(model()->rowCount()>0)
    {
        editAction->setEnabled(true);
        removeAction->setEnabled(true);
        removeAllAction->setEnabled(true);
    }
    else
    {
        editAction->setEnabled(false);
        removeAction->setEnabled(false);
        removeAllAction->setEnabled(false);
    }

    contextMenu->exec(e->globalPos());
    if (!contextMenu.isNull())
        delete contextMenu;
}
