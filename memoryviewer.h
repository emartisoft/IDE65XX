#ifndef MEMORYVIEWER_H
#define MEMORYVIEWER_H

#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QPoint>

#define ADRCOUNT    0x10000

class MemoryViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryViewer(QWidget *parent = nullptr);
    void poke(int startAddress, int endAddress, QColor color);
    void reset();
    void setLineWidth(int lineWidth);
    QString getCurrentAddressString();
    int getCurrentAddress();

private:
    bool memory[ADRCOUNT];
    QColor memoryColor[ADRCOUNT];
    int m_lineWidth;
    int currentAddress;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void currentAddressChanged(int address);
    void currentCoordinateChanged(int x, int y);
};

#endif // MEMORYVIEWER_H
