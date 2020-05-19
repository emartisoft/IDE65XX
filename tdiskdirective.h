#ifndef TDISKDIRECTIVE_H
#define TDISKDIRECTIVE_H

#include <QDialog>

namespace Ui {
class TDiskDirective;
}

class TDiskDirective : public QDialog
{
    Q_OBJECT

public:
    explicit TDiskDirective(QWidget *parent = nullptr);
    ~TDiskDirective();

    QString getDiskDirective();

private slots:
    void on_tDiskname_textEdited(const QString &arg1);

    void on_tFilename_textEdited(const QString &arg1);

    void on_tDiskId_textEdited(const QString &arg1);

    void on_bCancel_clicked();

    void on_bOk_clicked();

    void on_bAdd_clicked();

    void on_bRemove_clicked();

    void on_cDefine_currentIndexChanged(int index);

    void on_bClear_clicked();

    void on_bUp_clicked();

    void on_bDown_clicked();

private:
    Ui::TDiskDirective *ui;
    QString m_diskdirective;
    QStringList fl;
};

#endif // TDISKDIRECTIVE_H
