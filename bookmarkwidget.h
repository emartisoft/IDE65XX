#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QListView>
#include <QPointer>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include "delegate.h"
#include <QStandardItemModel>
#include <QStringList>

class BookmarkWidget : public QListView
{
    Q_OBJECT
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
    QAction *removeAction;
    QAction *removeAllAction;
    QAction *editAction;

    quint64 getLineNumber(int index);
    QString getFilepath(int index);
    QString getItemText(int index);
    QIcon getIcon(int index);
    void setTopText(int index, QString text);

public slots:
    void addItem(const QPixmap &pixmap, const QString &text, const QString &topText);
    void removeAll();
    void removeItem(int index);
    void setItemText(int index, QString text);
protected:
    void contextMenuEvent(QContextMenuEvent *e) override;
private:
    QPointer<QMenu> contextMenu;
    Delegate *delegate;

};

#endif // BOOKMARKWIDGET_H
