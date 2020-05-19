#include "popupmenu.h"
#include <QPushButton>

PopupMenu::PopupMenu(QPushButton *button, QWidget *parent) :
    QMenu(parent),
    button(button)
{
}

void PopupMenu::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    QPoint p = pos();
    QRect geo = button->geometry();
    this->move(p.x()+geo.width()-this->geometry().width(), p.y());
}
