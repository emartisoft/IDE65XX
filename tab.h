#ifndef TAB_H
#define TAB_H

#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QtWidgets/QMainWindow>
#include <QDockWidget>
#include <QPlainTextEdit>

#include "highlighter.h"
#include "codeeditor.h"

class Tab : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tab(QWidget *parent = nullptr);
    virtual ~Tab();

    QTextDocument *getCodeDocument();
    void saveCodeToFile(const QString &filePath, bool changeCodeModifiedFlag = true);
    bool loadCodeFromFile(const QString &filePath);
    QString getCurrentFilePath();
    void setFonts();
    CodeEditor *code;
    Highlighter *highlighter;

private:
    QString currentFilePath;
    QSettings settings;
    int oldBlockCount;

private slots:
    void blockCountChanged(int newBlockCount);



};

#endif // TAB_H
