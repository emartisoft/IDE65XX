#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QtCore>
#include "qhexedit2/qhexedit.h"

namespace Ui {
    class HexSearchDialog;
}

class HexSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HexSearchDialog(QHexEdit *hexEdit, QWidget *parent = 0);
    ~HexSearchDialog();
    qint64 findNext();
    Ui::HexSearchDialog *ui;

private slots:
    void on_pbFind_clicked();
    void on_pbReplace_clicked();
    void on_pbReplaceAll_clicked();

private:
    QByteArray getContent(int comboIndex, const QString &input);
    qint64 replaceOccurrence(qint64 idx, const QByteArray &replaceBa);

    QHexEdit *_hexEdit;
    QByteArray _findBa;
};

#endif // SEARCHDIALOG_H
