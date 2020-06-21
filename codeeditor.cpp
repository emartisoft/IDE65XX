#include "codeeditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QtMath>
#include <QToolTip>
#include "hint.h"

CodeEditor::CodeEditor(QWidget *parent) : BasePlainTextEdit(parent),  
    lineNumberArea(new LineNumberArea(this)),
    bookmarkImage(":/res/images/bookmark.png"),
    bookmarkAreaWidth(bookmarkImage.width())
{
    installEventFilter(this);
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setCursorWidth(2);

    connect(toogleBookmark, SIGNAL(triggered()), this, SLOT(setBookmarkOnCurrentLine()));
    setMouseTracking(true);
}

CodeEditor::~CodeEditor()
{

}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    bookmarkAreaWidth = bookmarkImage.width()*fontMetrics().height()/bookmarkImage.height();
    int space = 8 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + bookmarkAreaWidth;
    return space;
}

QMenu *CodeEditor::getContextMenu()
{
    return createMenu();
}

void CodeEditor::setCompleter(QCompleter *completer)
{
    if (c) c->disconnect(this);
    c = completer;
    if (!c) return;
    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->popup()->setFont(font());
    QObject::connect(c, QOverload<const QString &>::of(&QCompleter::activated), this, &CodeEditor::insertCompletion);
}

QCompleter *CodeEditor::completer() const
{
    return c;
}

void CodeEditor::repaintLineNumberArea()
{
    //repaint
    QRect lineNumberAreaRect(lineNumberArea->x(), lineNumberArea->y(),
                             lineNumberArea->width(), lineNumberArea->height());
    emit updateRequest(lineNumberAreaRect, 0);
}

QString CodeEditor::getHelpWord()
{
    // result may be tooltip showing text or under cursor word
    return (tooltipShowing) ? tooltipWord : textUnderCursor();
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::keyReleaseEvent(QKeyEvent *event)
{
    int initial_position;
    QTextCursor tc = textCursor();

    // Zoom
    if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_0)) // CTRL + 0
    {
            int d = getZoom();
            (d>0)?zoomIn(-d):zoomOut(d);
            resetZoom();
    }
    // insert or overwrite
    if(event->key() == Qt::Key_Insert)
    {
        setOverwriteMode(!overwriteMode());
        if(overwriteMode())
        {
            setCursorWidth(QFontMetrics(font()).horizontalAdvance('9') - 1); // cursor width

        }
        else
        {
            setCursorWidth(2);
        }
        emit overWriteModeChanged();
    }

    if(event->key() == Qt::Key_Tab)
    {
        if(tabSpace)
        {

            tc.deletePreviousChar();
            tc.insertText(QString(tabSpaceCount, ' '));
        }
    }

    if (event->key() == Qt::Key_BraceLeft) {
        tc.deletePreviousChar();
        initial_position = QPlainTextEdit::textCursor().position();
        QPlainTextEdit::insertPlainText("{\n\n}");
        tc = QPlainTextEdit::textCursor();

        tc.setPosition(initial_position+2);
        QPlainTextEdit::setTextCursor(tc);
    }

    if (event->key() == Qt::Key_ParenLeft) {
        tc.deletePreviousChar();
        initial_position = QPlainTextEdit::textCursor().position();
        QPlainTextEdit::insertPlainText("()");
        tc = QPlainTextEdit::textCursor();

        tc.setPosition(initial_position+1);
        QPlainTextEdit::setTextCursor(tc);
    }
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c) c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    const bool isShortcut = (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_Space); // CTRL+SPACE

    if (c && c->popup()->isVisible()) {
       switch (event->key()) {
           case Qt::Key_Return:
           case Qt::Key_Escape:
           case Qt::Key_Enter:
                event->ignore();
                return;
           default:
               break;
       }
    }
    else if (!c || !isShortcut)
    {
        switch (event->key())
        {
            case (Qt::Key_Enter) :
            case (Qt::Key_Return) :
            {
                QPlainTextEdit::keyPressEvent(event);
                QTextCursor cur = textCursor();
                cur.movePosition(QTextCursor::PreviousBlock);
                cur.movePosition(QTextCursor::StartOfBlock);
                cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
                QString str = cur.selectedText();   // get text from line

                emit AfterEnterSendLine(str); // signal

                int spcCount = 0;
                int tabCount = 0;
                int lstr = str.length();
                for(int i=0;i<lstr;i++)
                {
                    if(str[i] == ' ')
                    {
                            spcCount++;
                    }
                    else if(str[i] == '\t')
                    {
                            tabCount++;
                    }
                    else if ((str[i] != ' ')&&(str[i] != '\t'))
                    {
                            break;
                    }
                }
                insertPlainText(QString(tabCount, '\t')+QString(spcCount, ' '));
                return;
                break;
            }
        }
    }

    // bookmark
    if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_B)) // CTRL + B
    {
        setBookmarkOnCurrentLine();
    }
    // show find dialog signal
    else if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_F)) // CTRL + F
    {
        showFindDialog();
    }
    // uppercase
    else if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_U)) // CTRL + U
    {
        uppercaseSelectedCode();
    }
    // lowercase
    else if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_L)) // CTRL + L
    {
        lowercaseSelectedCode();
    }
    // comment
    else if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_Q)) // CTRL + Q
    {
        commentSelectedCode();
    }
    // uncomment
    else if( (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_R)) // CTRL + R
    {
        uncommentSelectedCode();
    }

    BasePlainTextEdit::keyPressEvent(event);

    const bool ctrlOrShift = event->modifiers().testFlag(Qt::ControlModifier) ||
                             event->modifiers().testFlag(Qt::ShiftModifier);

    if (!c || (ctrlOrShift && event->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || event->text().isEmpty()|| completionPrefix.length() < 2
                      || eow.contains(event->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }

    if(cAutoCompletion || isShortcut)
    {
        QRect cr = cursorRect();
        cr.setWidth(c->popup()->sizeHintForColumn(0)
                    + c->popup()->verticalScrollBar()->sizeHint().width());
        c->complete(cr);
    }

}

bool CodeEditor::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QPoint pos = helpEvent->pos();
        pos.setX(pos.x() - viewportMargins().left());
        pos.setY(pos.y() - viewportMargins().top());

        QTextCursor tcursor = cursorForPosition(pos);
        tcursor.select(QTextCursor::WordUnderCursor);

        int hint = Hint::getHint(tcursor.selectedText());

        if(hint>-1)
        {
            tooltipWord = Hint::hints[hint][0];
            QToolTip::showText(helpEvent->globalPos(), QString(""
                                                               "<table><tbody><tr><td><table style=\"width: 100%;\"><tbody><tr>"
                                                               "<td style=\"vertical-align: top;\"><h2><strong>%1</strong></h2>"
                                                               "</td></tr><tr><td><em>%2</em></td></tr>"
                                                               "<tr><td>%3</td></tr></tbody></table></td>"
                                                               "<td style=\"width: 48px; text-align: right; vertical-align: top;\">"
                                                               "<img src=\":/res/images/f1.png\" /></td></tr></tbody></table>"
                                                               ).arg(tooltipWord).arg(Hint::hints[hint][1]).arg(Hint::hints[hint][2]));
            tooltipShowing = true;
        }
        else
        {
            QToolTip::hideText();
            tooltipShowing = false;
        }
        return true;
    }
    return QPlainTextEdit::event(event);
}

bool CodeEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        if ((keyEvent->key() == Qt::Key_Tab) || (keyEvent->key() == Qt::Key_Backtab)) {
                bool shifttab = (keyEvent->key() == Qt::Key_Backtab);
                const QString tabChar = QChar('\t');

                QTextCursor tc = textCursor();
                QString currentLineText = tc.selectedText();

                if (!currentLineText.isEmpty())
                {
                  const QString newLine = QString::fromUtf8(QByteArray::fromHex(QByteArrayLiteral("e280a9")));
                  QString newText;

                  if (shifttab)
                  {
                      const int indentSize = tabChar == QStringLiteral("\t") ? 4 : tabChar.count();
                      newText = currentLineText.replace(QRegularExpression(newLine + QStringLiteral("(\\t| {1,") + QString::number(indentSize) + QStringLiteral("})")), QStringLiteral("\n"));
                      newText.remove(QRegularExpression(QStringLiteral("^(\\t| {1,") + QString::number(indentSize) + QStringLiteral("})")));
                  }
                  else
                  {
                      newText = currentLineText.replace(QRegularExpression(QRegularExpression::escape(newLine) + QStringLiteral("$")), QStringLiteral("\n"));
                      newText.replace(newLine, QStringLiteral("\n") + tabChar).prepend(tabChar);
                      newText.remove(QRegularExpression(QStringLiteral("\\t$")));
                  }

                  tc.insertText(newText);
                  tc.setPosition(tc.position() - newText.size(), QTextCursor::KeepAnchor);
                  setTextCursor(tc);
                  return true;
                }
                else if(!shifttab)
                {
                    tc.insertText("\t");
                }
                return true;
        }
    }
    return QPlainTextEdit::eventFilter(obj, event);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = palette().window().color().darker(128);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void CodeEditor::insertCompletion(const QString &completion)
{
    if (c->widget() != this) return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void CodeEditor::setBookmarkOnCurrentLine()
{
    int lineNumber = textCursor().blockNumber() + 1;
    if (lineNumber <= document()->lineCount())
    {
        lineNumber = document()->findBlockByLineNumber(lineNumber - 1).blockNumber() + 1;
        if (bookmarks.contains(lineNumber))
        {
            bookmarks.removeOne(lineNumber);
            emit bookmarksChanged(lineNumber, "", false);
        }
        else
        {
            bookmarks.append(lineNumber);
            emit bookmarksChanged(lineNumber, textCursor().block().text().trimmed().append(" "), true);
        }
        repaintLineNumberArea();
    }
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform,1);
    painter.fillRect(event->rect(), palette().window().color());

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::darkGray);
            painter.drawText(0, top - 2, lineNumberArea->width() - bookmarkAreaWidth, fontMetrics().height(), Qt::AlignRight, number);
        }

        if (bookmarks.contains(blockNumber + 1))
                        painter.drawPixmap(lineNumberArea->width() - bookmarkAreaWidth, top, bookmarkImage.width()*fontMetrics().height()/bookmarkImage.height(), fontMetrics().height(), bookmarkImage);

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::lineNumberAreaMousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    int lineNumber = 0;
    int sumHeight = 0;
    QTextBlock block = firstVisibleBlock();
    while (block.isValid() && event->y() > sumHeight)
    {
        sumHeight += blockBoundingGeometry(block).height();
        block = block.next();
        lineNumber++;
    }
    int vsbv = verticalScrollBar()->value();
    lineNumber += vsbv;

    if (lineNumber <= document()->lineCount())
    {
        lineNumber = document()->findBlockByLineNumber(lineNumber - 1).blockNumber() + 1;
        if (bookmarks.contains(lineNumber))
        {
            bookmarks.removeOne(lineNumber);
            emit bookmarksChanged(lineNumber, "", false);
        }
        else
        {
            bookmarks.append(lineNumber);
            moveCursor(QTextCursor::Start);
            for (int i=0;i<lineNumber-1;i++)
            {
                moveCursor(QTextCursor::Down);
            }

            setFocus();
            emit bookmarksChanged(lineNumber, textCursor().block().text().trimmed().append(" "), true);
        }

        repaintLineNumberArea();
    }

    verticalScrollBar()->setValue(vsbv);
}

