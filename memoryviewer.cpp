#include "memoryviewer.h"

MemoryViewer::MemoryViewer(QWidget *parent) : QWidget(parent),
    m_lineWidth(0x40),
    currentAddress(0x0000)
{
    reset();
}

void MemoryViewer::poke(int startAddress, int endAddress, QColor color)
{
    for(int i=startAddress;i<endAddress;i++)
    {
        memory[i] = true;
        memoryColor[i] = color;
    }
    update();
}

void MemoryViewer::reset()
{
    for(int i=0;i<ADRCOUNT;i++)
    {
        memory[i] = false;
        memoryColor[i] = QColor(0xff, 0xff, 0xff);
    }
    update();
}

void MemoryViewer::setLineWidth(int lineWidth)
{
    m_lineWidth = lineWidth;
    setFixedHeight(ADRCOUNT/lineWidth + 2);
    update();
}

QString MemoryViewer::getCurrentAddressString()
{
    return QString("$%1").arg(currentAddress,4,16, QLatin1Char('0'));
}

int MemoryViewer::getCurrentAddress()
{
    return currentAddress;
}

void MemoryViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // frame
    painter.setPen(QPen(Qt::darkGray));
    painter.drawLine(0,0,m_lineWidth+1,0);
    painter.drawLine(m_lineWidth+1,0,m_lineWidth+1,ADRCOUNT/m_lineWidth+1);
    painter.drawLine(m_lineWidth+1,ADRCOUNT/m_lineWidth+1,0,ADRCOUNT/m_lineWidth+1);
    painter.drawLine(0,ADRCOUNT/m_lineWidth+1,0,0);

    painter.setFont(QFont(font().family(),8));
    for(int y=m_lineWidth*16;y<ADRCOUNT;y+=m_lineWidth*16)
    {
        painter.drawLine(m_lineWidth+1,y/m_lineWidth+1,m_lineWidth+8,y/m_lineWidth+1);
        painter.drawText(m_lineWidth+10,y/m_lineWidth+1+painter.fontMetrics().height()/4,QString("%1-%2").arg(y,4,16, QLatin1Char('0')).arg(y+m_lineWidth-1,4,16, QLatin1Char('0')).toUpper());
    }

    // memory
    for(int i=0;i<ADRCOUNT;i++)
    {
        if(memory[i])
        {
            painter.setPen(QPen(memoryColor[i]));
            painter.drawPoint(i % m_lineWidth+1, i/m_lineWidth+1);
        }
    }
}

void MemoryViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        int mx = event->x();
        int my = event->y();
        emit currentCoordinateChanged(mx, my);
        if ((my<1)||(my>ADRCOUNT/m_lineWidth)||(mx<1)||(mx > m_lineWidth)) return;
        mx--;
        my--;
        currentAddress = my*m_lineWidth+mx;
        emit currentAddressChanged(currentAddress);
    }
}
