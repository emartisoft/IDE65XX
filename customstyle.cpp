#include "customstyle.h"
#include "common.h"
#include <QComboBox>
#include <QPainter>
#include <QPushButton>
#include <QStyleFactory>
#include <QDir>

CustomStyle::CustomStyle() : QProxyStyle(QStyleFactory::create("fusion")),
    settings(Common::appConfigDir()+"/settings.ini", QSettings::IniFormat)
{
    setObjectName("CustomStyle");
    customPaletteColors[0] = settings.value("CustomBase", QColor(0xc7, 0xc9, 0xcd)).value<QColor>();
    customPaletteColors[1] = settings.value("CustomBackground", QColor(0xf0, 0xf0, 0xf0)).value<QColor>();
    customPaletteColors[2] = settings.value("CustomHighlights", QColor(0xb1, 0xb1, 0xb1)).value<QColor>();
    customPaletteColors[3] = settings.value("CustomBrightText", QColor(255,255,255)).value<QColor>();
    customPaletteColors[4] = settings.value("CustomDisable", QColor(64,64,64)).value<QColor>();

    customTexturesBackgroundImg = settings.value("CustomBackgroundTexture", ":/res/style/commodore_background.png").toString();
    customTexturesButtonImg = settings.value("CustomButtonTexture", ":/res/style/commodore_button.png").toString();

}

QPalette CustomStyle::standardPalette() const
{
    if (!m_standardPalette.isBrushSet(QPalette::Disabled, QPalette::Mid))
    {
        QImage buttonImage(customTexturesButtonImg);
        QImage backgroundImage(customTexturesBackgroundImg);
        QPalette palette(customPaletteColors[0]);
        QImage midImage = buttonImage.convertToFormat(QImage::Format_RGB32);

        QPainter painter;
        painter.begin(&midImage);
        painter.setPen(Qt::NoPen);
        painter.fillRect(midImage.rect(), QColor(0x0, 0x0, 0x0, 0x10));
        painter.end();

        QPainter painterback;
        painterback.begin(&backgroundImage);
        painterback.setPen(Qt::NoPen);
        painterback.fillRect(backgroundImage.rect(), QColor(0x0, 0x0, 0x0, 0x10));
        painterback.end();

        setTexture(palette, QPalette::Button, buttonImage);
        setTexture(palette, QPalette::Mid, midImage);
        setTexture(palette, QPalette::Window, backgroundImage);

        palette.setBrush(QPalette::BrightText, customPaletteColors[3]);
        palette.setBrush(QPalette::Base, customPaletteColors[1]);
        palette.setBrush(QPalette::Highlight, customPaletteColors[2]);
        QBrush brush = palette.windowText();
        brush.setColor(customPaletteColors[4]);

        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);

        m_standardPalette = palette;
    }
    return m_standardPalette;
}

void CustomStyle::polish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget)
            )
        widget->setAttribute(Qt::WA_Hover, true);
}

void CustomStyle::unpolish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget)
            )
        widget->setAttribute(Qt::WA_Hover, false);
}

void CustomStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (element)
    {
        case PE_PanelButtonCommand:
        {
            int delta = (option->state & State_MouseOver) ? 64 : 0;
            QColor slightlyOpaqueBlack(0, 0, 0xf0, 255);
            QColor semiTransparentWhite(255, 255, 255, 127 + delta);
            QColor semiTransparentBlack(0, 0, 0, 127 - delta);

            int x, y, width, height;
            option->rect.getRect(&x, &y, &width, &height);
            QPainterPath roundRect = roundRectPath(option->rect);
            int radius = qMin(width, height) / 2;
            QBrush brush;
            bool darker;

            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption
                    && (buttonOption->features & QStyleOptionButton::Flat)) {
                brush = option->palette.window();
                darker = (option->state & (State_Sunken | State_On));
            } else {
                if (option->state & (State_Sunken | State_On)) {
                    brush = option->palette.mid();
                    darker = !(option->state & State_Sunken);
                } else {
                    brush = option->palette.button();
                    darker = false;
                }
            }
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->fillPath(roundRect, brush);
            if (darker)
                painter->fillPath(roundRect, slightlyOpaqueBlack);
            int penWidth;
            if (radius < 10)
                penWidth = 3;
            else if (radius < 20)
                penWidth = 5;
            else
                penWidth = 7;

            QPen topPen(semiTransparentWhite, penWidth);
            QPen bottomPen(semiTransparentBlack, penWidth);

            if (option->state & (State_Sunken | State_On))
                qSwap(topPen, bottomPen);
            int x1 = x;
            int x2 = x + radius;
            int x3 = x + width - radius;
            int x4 = x + width;

            if (option->direction == Qt::RightToLeft) {
                qSwap(x1, x4);
                qSwap(x2, x3);
            }

            QPolygon topHalf;
            topHalf << QPoint(x1, y)
                    << QPoint(x4, y)
                    << QPoint(x3, y + radius)
                    << QPoint(x2, y + height - radius)
                    << QPoint(x1, y + height);

            painter->setClipPath(roundRect);
            painter->setClipRegion(topHalf, Qt::IntersectClip);
            painter->setPen(topPen);
            painter->drawPath(roundRect);

            QPolygon bottomHalf = topHalf;
            bottomHalf[0] = QPoint(x4, y + height);

            painter->setClipPath(roundRect);
            painter->setClipRegion(bottomHalf, Qt::IntersectClip);
            painter->setPen(bottomPen);
            painter->drawPath(roundRect);

            painter->setPen(option->palette.windowText().color());
            painter->setClipping(false);
            painter->drawPath(roundRect);

            painter->restore();
        }
        break;

    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

void CustomStyle::drawControl(QStyle::ControlElement control, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (control) {
    case CE_PushButtonLabel:
        {
            QStyleOptionButton myButtonOption;
            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption) {
                myButtonOption = *buttonOption;
                if (myButtonOption.palette.currentColorGroup()
                        != QPalette::Disabled) {
                    if (myButtonOption.state & (State_Sunken | State_On)) {
                        myButtonOption.palette.setBrush(QPalette::ButtonText,
                                myButtonOption.palette.brightText());
                    }
                }
            }
            QProxyStyle::drawControl(control, &myButtonOption, painter, widget);
        }
        break;
    default:
        QProxyStyle::drawControl(control, option, painter, widget);
    }
}

void CustomStyle::setTexture(QPalette &palette, QPalette::ColorRole role, const QImage &image)
{
    for (int i = 0; i < QPalette::NColorGroups; ++i) {
        QBrush brush(image);
        brush.setColor(palette.brush(QPalette::ColorGroup(i), role).color());
        palette.setBrush(QPalette::ColorGroup(i), role, brush);
    }
}

QPainterPath CustomStyle::roundRectPath(const QRect &rect)
{
    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    QPainterPath path;
    path.moveTo(x2, y1 );
    path.lineTo(x1, y1);
    path.lineTo(x1, y2);
    path.lineTo(x2, y2);
    path.closeSubpath();
    return path;
}
