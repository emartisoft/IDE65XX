#include "tdiskdirective.h"
#include "ui_tdiskdirective.h"

TDiskDirective::TDiskDirective(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDiskDirective)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

TDiskDirective::~TDiskDirective()
{
    delete ui;
}

QString TDiskDirective::getDiskDirective()
{
    return m_diskdirective;
}

void TDiskDirective::on_tDiskname_textEdited(const QString &arg1)
{
    ui->tDiskname->setText(arg1.toUpper());
}

void TDiskDirective::on_tFilename_textEdited(const QString &arg1)
{
    ui->tFilename->setText(arg1.toUpper());
}

void TDiskDirective::on_tDiskId_textEdited(const QString &arg1)
{
    ui->tDiskId->setText(arg1.toUpper());
}

void TDiskDirective::on_bCancel_clicked()
{
    close();
}

void TDiskDirective::on_bOk_clicked()
{
    QString sDiskFilename = ui->tDiskFilename->text();
    QString sDiskId = ui->tDiskId->text();
    QString sDiskName = ui->tDiskname->text();
    QString sDiskFormat = ui->cDiskFormat->currentText();

    if(sDiskFilename.isEmpty()){ui->tDiskFilename->setFocus(); return;}
    if(sDiskId.isEmpty()){ui->tDiskId->setFocus(); return;}
    if(sDiskName.isEmpty()){ui->tDiskname->setFocus(); return;}

    m_diskdirective = QString("\n.disk [filename=\"%1\", name=\"%2\", id=\"%3\", format=\"%4\"").arg(sDiskFilename).arg(sDiskName).arg(sDiskId).arg(sDiskFormat);
    if(ui->cShowInfo->isChecked()) m_diskdirective += ", showInfo";
    m_diskdirective += "]\n{\n";

    for(int i=0;i<fl.count();i++)
    {
        m_diskdirective += "\t" + fl[i];
        if(i != (fl.count()-1)) m_diskdirective += ",\n";
    }

    m_diskdirective += "\n}\n\n";

    if(ui->cInsertBasicUpstart2->isChecked())
        m_diskdirective += "BasicUpstart2(start)\nstart:\n\n";

    close();
}

void TDiskDirective::on_bAdd_clicked()
{
    QString item;
    QString sFilename = ui->tFilename->text();
    QString sType = ui->cType->currentText();
    QString sDefine = ui->tDefine->text();

    if(sFilename.isEmpty()){ ui->tFilename->setFocus(); return; }

    sFilename = sFilename.leftJustified(16, ' ');

    switch (ui->cDefine->currentIndex()) {
        case 0:
                sDefine = "";
        break;

        case 1:
                sDefine = QString(", segments=\"%1\"").arg(sDefine);
        break;

        case 2:
                if(sType=="sid")
                {
                    sDefine = QString(", sidFiles=\"%1\"").arg(sDefine);
                    sType = "prg";
                }
                else
                    sDefine = QString(", prgFiles=\"%1\"").arg(sDefine);
        break;
    }

    if(ui->cLocked->isChecked()) sType += "<";

    item = QString("[name=\"%1\", type=\"%2\"").arg(sFilename).arg(sType);
    if(!sDefine.isEmpty()) item += sDefine;
    if(ui->cHide->isChecked()) item += ", hide";
    item += "]";

    fl << item;
    ui->listFiles->clear();
    ui->listFiles->addItems(fl);
}

void TDiskDirective::on_bRemove_clicked()
{
    if(ui->listFiles->selectedItems().empty()) return;
    fl.removeAt(ui->listFiles->currentRow());
    ui->listFiles->clear();
    ui->listFiles->addItems(fl);
}

void TDiskDirective::on_cDefine_currentIndexChanged(int index)
{
    ui->tDefine->setEnabled(index!=0);
}

void TDiskDirective::on_bClear_clicked()
{
    ui->listFiles->clear();
    fl.clear();
}

void TDiskDirective::on_bUp_clicked()
{
    if(ui->listFiles->selectedItems().empty()) return;
    int idx = ui->listFiles->currentRow();
    if(idx==0) return;
    fl.move(idx, idx-1);
    ui->listFiles->clear();
    ui->listFiles->addItems(fl);
    ui->listFiles->setCurrentRow(idx-1);
}

void TDiskDirective::on_bDown_clicked()
{
    if(ui->listFiles->selectedItems().empty()) return;
    int idx = ui->listFiles->currentRow();
    if(idx==fl.count()-1) return;
    fl.move(idx, idx+1);
    ui->listFiles->clear();
    ui->listFiles->addItems(fl);
    ui->listFiles->setCurrentRow(idx+1);
}
