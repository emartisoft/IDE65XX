#ifndef TFILEDIRECTIVE_H
#define TFILEDIRECTIVE_H

#include <QDialog>

namespace Ui {
class TFileDirective;
}

class TFileDirective : public QDialog
{
    Q_OBJECT

public:
    explicit TFileDirective(QWidget *parent = nullptr);
    ~TFileDirective();
    QString getFileDirective();

private slots:
    void on_bCancel_clicked();
    void on_bOk_clicked();

private:
    Ui::TFileDirective *ui;
    QString m_filedirective;
};

#endif // TFILEDIRECTIVE_H
