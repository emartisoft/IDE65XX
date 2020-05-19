#include "tfiledirective.h"
#include "ui_tfiledirective.h"

TFileDirective::TFileDirective(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TFileDirective)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

TFileDirective::~TFileDirective()
{
    delete ui;
}

QString TFileDirective::getFileDirective()
{
    return m_filedirective;
}

void TFileDirective::on_bCancel_clicked()
{
    close();
}

void TFileDirective::on_bOk_clicked()
{
    QString sFilename = ui->tFilename->text();
    QString sSegment = ui->tSegment->text();
    QString sType = ui->cType->currentText();

    if(sFilename.isEmpty()) {
        ui->tFilename->setFocus();
        return;
    }

    if(sSegment.isEmpty()) {
        ui->tSegment->setFocus();
        return;
    }

    QString sBetween = "type=\""+sType+"\"";
    if (ui->cMbfiles->isChecked()) sBetween = "mbfiles";

    m_filedirective = QString("\n.file [name=\"%1.%3\", %4, segments=\"%2\"]\n.segment %2[]\n").arg(sFilename).arg(sSegment).arg(sType).arg(sBetween);

    if(ui->cInsertBasicUpstart2->isChecked())
        m_filedirective += "BasicUpstart2(start)\nstart:\n\n";

    close();
}
