#ifndef CHOOSETOPIC_H
#define CHOOSETOPIC_H

#include <QDialog>

namespace Ui {
class ChooseTopic;
}

class ChooseTopic : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseTopic(QWidget *parent = nullptr);
    ~ChooseTopic();

    void setWord(QString word);
    QString getWord();

private:
    Ui::ChooseTopic *ui;
};

#endif // CHOOSETOPIC_H
