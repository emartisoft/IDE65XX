#include "choosetopic.h"
#include "ui_choosetopic.h"

ChooseTopic::ChooseTopic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseTopic)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

ChooseTopic::~ChooseTopic()
{
    delete ui;
}

void ChooseTopic::setWord(QString word)
{
    ui->lInfo->setText(ui->lInfo->text().replace("<text>", word));
    if(word=="define")
    {
        ui->lwWords->addItems(QStringList()<<"#define | Preprocessor directive"<<".define | Assembler directive");
    }
    else if (word=="import") {
        ui->lwWords->addItems(QStringList()<<"#import | Preprocessor directive"<<".import binary | Assembler directive"
                              <<".import c64 | Assembler directive"<<".import source | Assembler directive"
                              <<".import text | Assembler directive");
    }
    else if (word=="if") {
        ui->lwWords->addItems(QStringList()<<"#if | Preprocessor directive"<<".if | Assembler directive");
    }
    else if (word=="importonce") {
        ui->lwWords->addItems(QStringList()<<"#importonce | Preprocessor directive"<<".importonce | Assembler directive");
    }
    ui->lwWords->setCurrentRow(0);
}

QString ChooseTopic::getWord()
{
    QString result = ui->lwWords->currentItem()->text().remove(".");
    return result.split(" | ")[0];
}
