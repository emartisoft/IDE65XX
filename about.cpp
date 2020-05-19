#include "about.h"
#include "ui_about.h"

#include <QMessageBox>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::on_bAboutQt_clicked()
{
    QMessageBox::aboutQt(this);
}
