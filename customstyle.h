#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H

#include <QProxyStyle>
#include <QStyleFactory>
#include <QPainterPath>
#include <QSettings>

class CustomStyle : public QProxyStyle
{
    Q_OBJECT
public:
    CustomStyle();
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
    QColor customPaletteColors[5];
    QString customTexturesBackgroundImg;
    QString customTexturesButtonImg;

    QSettings settings;

};

#endif // CUSTOMSTYLE_H
