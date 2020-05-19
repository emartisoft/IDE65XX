#ifndef DARKFUSIONSTYLE_H
#define DARKFUSIONSTYLE_H

#include <QProxyStyle>
#include <QStyleFactory>

class DarkFusionStyle : public QProxyStyle
{
    Q_OBJECT
public:
    DarkFusionStyle(QStyle *style = QStyleFactory::create("fusion"));
    void polish(QPalette &palette) override;
};

#endif // DARKFUSIONSTYLE_H
