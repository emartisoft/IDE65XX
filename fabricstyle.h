#ifndef FABRICSTYLE_H
#define FABRICSTYLE_H

#include <QProxyStyle>
#include <QPalette>
#include <QPainterPath>

class FabricStyle : public QProxyStyle
{
    Q_OBJECT
public:
    FabricStyle();
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

#endif // FABRICSTYLE_H
