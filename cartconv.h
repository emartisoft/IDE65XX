#ifndef CARTCONV_H
#define CARTCONV_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QProcess>
#include "common.h"

namespace Ui {
class CartConv;
}

class CartConv : public QDialog
{
    Q_OBJECT

public:
    explicit CartConv(QWidget *parent = nullptr);
    ~CartConv();
    void setCartConvFilename(QString &value);
    void setPrgFilePath(QString prgFilePath);
    void setCartName(QString name);
    void clear();

private slots:
    void on_bPrgFilePath_clicked();

    void on_bConvert_clicked();

private:
    Ui::CartConv *ui;
    QString cartconvFilename;
    QStringList prgFileNames;
};

#endif // CARTCONV_H
