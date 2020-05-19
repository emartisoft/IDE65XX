#include "darkfusionstyle.h"

DarkFusionStyle::DarkFusionStyle(QStyle *style) :
    QProxyStyle(style)
{

}

void DarkFusionStyle::polish(QPalette &palette)
{
    QColor darkGray(96, 96, 96);
    QColor blue(0x61, 0x61, 0xd0);
    QColor whitesmoke(192, 192, 192, 32);

    palette.setColor(QPalette::Window, darkGray);
    palette.setColor(QPalette::WindowText, Qt::lightGray);
    palette.setColor(QPalette::Base, QColor(64, 64, 64));
    palette.setColor(QPalette::AlternateBase, darkGray);
    palette.setColor(QPalette::ToolTipBase, blue);
    palette.setColor(QPalette::ToolTipText, Qt::lightGray);
    palette.setColor(QPalette::Text, Qt::gray);
    palette.setColor(QPalette::Button, darkGray);
    palette.setColor(QPalette::ButtonText, QColor(20,20,20));
    palette.setColor(QPalette::Link, blue.darker());
    palette.setColor(QPalette::Highlight, blue);
    palette.setColor(QPalette::HighlightedText, Qt::black);

    palette.setColor(QPalette::Active, QPalette::Button,  QColor(128, 128, 128));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, whitesmoke.lighter());
    palette.setColor(QPalette::Disabled, QPalette::WindowText, whitesmoke.lighter());
    palette.setColor(QPalette::Disabled, QPalette::Text, whitesmoke.lighter());
    palette.setColor(QPalette::Disabled, QPalette::Light, whitesmoke.lighter());

    palette.setColor(QPalette::Light, QColor(192, 192, 192, 32));
    palette.setColor(QPalette::Mid, whitesmoke.darker());
    palette.setColor(QPalette::Dark, QColor("#303030").lighter());

}
