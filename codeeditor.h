#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "baseplaintextedit.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QCompleter;
QT_END_NAMESPACE

class LineNumberArea;

class CodeEditor : public BasePlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void lineNumberAreaMousePressEvent(QMouseEvent *event);
    int lineNumberAreaWidth();
    QMenu *getContextMenu();
    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    QString textUnderCursor() const;
    void repaintLineNumberArea();
    QList<int> bookmarks;
    QString getHelpWord();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void highlightCurrentLine();
    void insertCompletion(const QString &completion);
    void setBookmarkOnCurrentLine();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    QCompleter *c = nullptr;
    QPixmap bookmarkImage;
    int bookmarkAreaWidth;
    QString tooltipWord;
    bool tooltipShowing;

signals:
    void bookmarksChanged(quint64 lineNumber, QString text, bool isAdded);
    void updateLineNumber(quint64 currentLine, int delta);
    void showFindDialog();
    void overWriteModeChanged();
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {

    }

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        codeEditor->lineNumberAreaMousePressEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif
