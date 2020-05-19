#include "tab.h"

Tab::Tab(QWidget *parent) :
    QMainWindow(parent),
    code(new CodeEditor()),
    currentFilePath(""),
    settings("IDE65xx Project", "IDE65xx")

{
    code->setWordWrapMode(QTextOption::NoWrap);
    connect(code->document(), SIGNAL(blockCountChanged(int)), this, SLOT(blockCountChanged(int)));

    setCentralWidget(code);
    setFonts();

    restoreGeometry(settings.value("tabgeometry").toByteArray());
    restoreState(settings.value("tabwindowstate").toByteArray());
}

void Tab::setFonts()
{
    QFont codeFont;
    codeFont.setPointSize(settings.value("fontsize", 12).toInt());
    codeFont.setFamily(settings.value("fontfamily", QString("Ubuntu Mono")).value<QString>());
    codeFont.setStyleHint(QFont::Monospace);
    code->setFont(codeFont);
}

void Tab::blockCountChanged(int newBlockCount)
{
    emit code->updateLineNumber(code->textCursor().blockNumber()+1, newBlockCount - oldBlockCount);
    oldBlockCount = newBlockCount;
}

QTextDocument * Tab::getCodeDocument()
{
    return code->document();
}

QString Tab::getCurrentFilePath()
{
    return currentFilePath;
}

void Tab::saveCodeToFile(const QString &filePath, bool changeCodeModifiedFlag)
{
    QFile outfile;
    outfile.setFileName(filePath);
    outfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outfile);

    out << code->toPlainText();
    if (changeCodeModifiedFlag) {
        currentFilePath = filePath;
        getCodeDocument()->setModified(false);

    }
    outfile.close();
}

bool Tab::loadCodeFromFile(const QString &filePath)
{
    QFile file;
    file.setFileName(filePath);
    if(!file.open(QIODevice::ReadOnly)) return false;
    QTextStream text(&file);
    QString source = text.readAll();
    code->setPlainText(source);
    currentFilePath = filePath;
    code->document()->setModified(false);
    file.close();
    return true;
}

Tab::~Tab()
{
    delete code;
}
