#include "about.h"
#include "ui_about.h"

#include <QMessageBox>
#include "version.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    ui->version->setText("IDE 65XX " + QString(VER_FILEVERSION_STR));
}

About::~About()
{
    delete ui;
}

void About::on_bAboutQt_clicked()
{
    QMessageBox::aboutQt(this);
}
