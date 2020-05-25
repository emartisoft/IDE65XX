#include "baseplaintextedit.h"

#define ZOOM    4

BasePlainTextEdit::BasePlainTextEdit(QWidget *parent): QPlainTextEdit(parent)
{
    commentAction = new QAction(QIcon(":/res/images/comment.png"), tr("Comment"), this);
    commentAction->setShortcut(Qt::CTRL|Qt::Key_Slash);
    connect(commentAction, SIGNAL(triggered()), this, SLOT(commentSelectedCode()));

    uncommentAction = new QAction(QIcon(":/res/images/removecomment.png"), tr("Remove Comment"), this);
    uncommentAction->setShortcut(Qt::CTRL|Qt::Key_R);
    connect(uncommentAction, SIGNAL(triggered()), this, SLOT(uncommentSelectedCode()));

    undoAction = new QAction(QIcon(":/res/images/undo.png"), tr("Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

    redoAction = new QAction(QIcon(":/res/images/redo.png"), tr("Redo"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

    cutAction = new QAction(QIcon(":/res/images/cut.png"), tr("Cut"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(QIcon(":/res/images/copy.png"), tr("Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(QIcon(":/res/images/paste.png"), tr("Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    deleteAction = new QAction(QIcon(":/res/images/erase.png"), tr("Delete"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSelected()));

    selectAllAction = new QAction(QIcon(":/res/images/selectall.png"), tr("Select All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

    lowercaseAction = new QAction(QIcon(":/res/images/lowercase.png"), tr("Lowercase"), this);
    lowercaseAction->setShortcut(Qt::CTRL|Qt::Key_L);
    connect(lowercaseAction, SIGNAL(triggered()), this, SLOT(lowercaseSelectedCode()));

    uppercaseAction = new QAction(QIcon(":/res/images/uppercase.png"), tr("Uppercase"), this);
    uppercaseAction->setShortcut(Qt::CTRL|Qt::Key_U);
    connect(uppercaseAction, SIGNAL(triggered()), this, SLOT(uppercaseSelectedCode()));

    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    connect(this, SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));

    toogleBookmark = new QAction(QIcon(":/res/images/bookmark.png"), "Toggle Bookmark", this);
    toogleBookmark->setShortcut(Qt::CTRL|Qt::Key_B);

    resetZoom();
}

BasePlainTextEdit::~BasePlainTextEdit()
{
    delete commentAction;
    delete uncommentAction;
    delete undoAction;
    delete redoAction;
    delete cutAction;
    delete copyAction;
    delete pasteAction;
    delete deleteAction;
    delete selectAllAction;
    delete uppercaseAction;
    delete lowercaseAction;
    delete toogleBookmark;
}

QMenu *BasePlainTextEdit::createMenu()
{
    QMenu *menu = new QMenu;
    QTextCursor textCursor = this->textCursor();

    //! if nothing selected
    if (textCursor.selectionEnd() - textCursor.selectionStart() <= 0)
    {
        uppercaseAction->setEnabled(false);
        lowercaseAction->setEnabled(false);
        cutAction->setEnabled(false);
        copyAction->setEnabled(false);
        deleteAction->setEnabled(false);
    }
    else
    {
        uppercaseAction->setEnabled(true);
        lowercaseAction->setEnabled(true);
        cutAction->setEnabled(true);
        copyAction->setEnabled(true);
        deleteAction->setEnabled(true);
    }

    undoAction->setVisible(!isReadOnly());
    redoAction->setVisible(!isReadOnly());
    cutAction->setVisible(!isReadOnly());
    pasteAction->setVisible(!isReadOnly());
    deleteAction->setVisible(!isReadOnly());

    menu->addAction(commentAction);
    menu->addAction(uncommentAction);
    menu->addSeparator();
    menu->addAction(undoAction);
    menu->addAction(redoAction);
    menu->addSeparator();
    menu->addAction(cutAction);
    menu->addAction(copyAction);
    menu->addAction(pasteAction);
    menu->addAction(deleteAction);
    menu->addSeparator();
    menu->addAction(selectAllAction);
    menu->addSeparator();
    menu->addAction(uppercaseAction);
    menu->addAction(lowercaseAction);
    menu->addSeparator();
    menu->addAction(toogleBookmark);
    return menu;
}

int BasePlainTextEdit::getZoom()
{
    return zoom;
}

void BasePlainTextEdit::resetZoom()
{
    zoom = 0;
}

void BasePlainTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMouseEvent leftClick(QEvent::MouseButtonPress, e->pos(), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mousePressEvent(&leftClick);
    contextMenu = createMenu();
    contextMenu->exec(e->globalPos());
    if (!contextMenu.isNull())
        delete contextMenu;
}

void BasePlainTextEdit::wheelEvent(QWheelEvent *e)
{
    if( e->modifiers() == Qt::ControlModifier )
    {
        if(e->delta() > 0)
        {

            if(zoom < ZOOM){
                zoomIn();
                zoom++;
            }
        }
        else
        {

            if(zoom > -ZOOM){
                zoomOut();
                zoom--;
            }
        }
    }
    QPlainTextEdit::wheelEvent(e);
}

void BasePlainTextEdit::commentSelectedCode()
{
    QTextCursor tc = textCursor();
    QString selected = tc.selectedText();
    if(selected.isEmpty())
    {
        tc.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 0);
        setTextCursor(tc);
        insertPlainText("//"); // ------->   // text
    }
    else
    {
        selected.insert(0, QString("/*"));
        selected.insert(selected.length(), QString("*/"));
        tc.insertText(selected); // ------->   /* text */
    }

}

void BasePlainTextEdit::uncommentSelectedCode()
{
    QTextCursor tc = textCursor();
    QString selected = tc.selectedText();
    if(selected.isEmpty())
    {
        tc.select(QTextCursor::LineUnderCursor);
        selected = tc.selectedText();

        int p = selected.indexOf("//");
        if(p > -1) selected.remove(p,2);
        tc.insertText(selected);
    }
    else
    {
        selected.remove("/*");
        selected.remove("*/");
        tc.insertText(selected);
    }
}

void BasePlainTextEdit::deleteSelected()
{
    textCursor().removeSelectedText();
}

void BasePlainTextEdit::uppercaseSelectedCode()
{
    textCursor().insertText(textCursor().selectedText().toUpper());
}

void BasePlainTextEdit::lowercaseSelectedCode()
{
    textCursor().insertText(textCursor().selectedText().toLower());
}
