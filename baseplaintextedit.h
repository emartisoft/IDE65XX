#ifndef BASEPLAINTEXTEDIT_H
#define BASEPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QTextBlock>
#include <QTextStream>
#include <QPointer>

class BasePlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    BasePlainTextEdit(QWidget *parent = nullptr);
    ~BasePlainTextEdit();

    QMenu *createMenu();
    int getZoom();
    void resetZoom();
    QAction *toogleBookmark;

protected:
    void contextMenuEvent(QContextMenuEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private:
    QPointer<QMenu> contextMenu;
    QAction *commentAction;
    QAction *uncommentAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectAllAction;
    QAction *uppercaseAction;
    QAction *lowercaseAction;
    int zoom;

public slots:
    void commentSelectedCode();
    void uncommentSelectedCode();
    void deleteSelected();
    void uppercaseSelectedCode();
    void lowercaseSelectedCode();

};

#endif // BASEPLAINTEXTEDIT_H
