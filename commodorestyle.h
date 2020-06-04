#ifndef COMMODORESTYLE_H
#define COMMODORESTYLE_H

#include <QProxyStyle>
#include <QPalette>
#include <QPainterPath>

QT_BEGIN_NAMESPACE
class QPainterPath;
QT_END_NAMESPACE

class CommodoreStyle : public QProxyStyle
{
    Q_OBJECT
public:
    CommodoreStyle();
    QPalette standardPalette() const override;
    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const override;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;
private:
    static void setTexture(QPalette &palette, QPalette::ColorRole role,
                           const QImage &image);
    static QPainterPath roundRectPath(const QRect &rect);
    mutable QPalette m_standardPalette;

};

#endif // COMMODORESTYLE_H
