#include "delegate.h"
#include <QPainter>

Delegate::Delegate(QObject *parent) : QStyledItemDelegate(parent),
    m_ptr(new DelegatePrivate)
{
}

Delegate::~Delegate()
{
	delete m_ptr;
}

QSize Delegate::iconSize() const
{
	return m_ptr->iconSize;
}

void Delegate::setIconSize(int width, int height)
{
	m_ptr->iconSize = QSize(width, height);
}

QMargins Delegate::contentsMargins() const
{
	return m_ptr->margins;
}

void Delegate::setContentsMargins(int left, int top, int right, int bottom)
{
	m_ptr->margins = QMargins(left, top, right, bottom);
}

int Delegate::horizontalSpacing() const
{
	return m_ptr->spacingHorizontal;
}

void Delegate::setHorizontalSpacing(int spacing)
{
	m_ptr->spacingHorizontal = spacing;
}

int Delegate::verticalSpacing() const
{
	return m_ptr->spacingVertical;
}

void Delegate::setVerticalSpacing(int spacing)
{
	m_ptr->spacingVertical = spacing;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem opt(option);
	initStyleOption(&opt, index);

	const QPalette &palette(opt.palette);
	const QRect &rect(opt.rect);
    const QRect &contentRect(rect.adjusted(m_ptr->margins.left(), m_ptr->margins.top(), -m_ptr->margins.right(), -m_ptr->margins.bottom()));
	const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
	const bool hasIcon = !opt.icon.isNull();
	const int bottomEdge = rect.bottom();
	QFont f(opt.font);
    f.setPointSize(opt.font.pointSize()-1);   // top text font size
	painter->save();
	painter->setClipping(true);
	painter->setClipRect(rect);
	painter->setFont(opt.font);

	// Draw background
	painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                          palette.window().color().darker(96) :
                          palette.base().color());

	// Draw bottom line
	painter->setPen(lastIndex ? palette.dark().color()
							  : palette.mid().color());
	painter->drawLine(lastIndex ? rect.left() : m_ptr->margins.left(),
					  bottomEdge, rect.right(), bottomEdge);

    // Draw text icon
	if (hasIcon)
		painter->drawPixmap(contentRect.left(), contentRect.top(),
							opt.icon.pixmap(m_ptr->iconSize));

    // Draw topText
    QRect topTextRect(m_ptr->topTextBox(opt, index));

    topTextRect.moveTo(m_ptr->margins.left() + m_ptr->iconSize.width()
						 + m_ptr->spacingHorizontal, contentRect.top());

	painter->setFont(f);
	painter->setPen(palette.text().color());
    painter->drawText(topTextRect, Qt::TextSingleLine,
					  index.data(Qt::UserRole).toString());

    // Draw text
    QRect textRect(m_ptr->textBox(opt));

    textRect.moveTo(topTextRect.left(), topTextRect.bottom()
					   + m_ptr->spacingVertical);

    painter->setFont(opt.font);
	painter->setPen(palette.windowText().color());
    painter->drawText(textRect, Qt::TextSingleLine, opt.text);

	painter->restore();
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option,
						 const QModelIndex &index) const
{
	QStyleOptionViewItem opt(option);
	initStyleOption(&opt, index);

    int textHeight = m_ptr->topTextBox(opt, index).height()
            + m_ptr->spacingVertical + m_ptr->textBox(opt).height();
	int iconHeight = m_ptr->iconSize.height();
	int h = textHeight > iconHeight ? textHeight : iconHeight;

	return QSize(opt.rect.width(), m_ptr->margins.top() + h
				 + m_ptr->margins.bottom());
}

DelegatePrivate::DelegatePrivate() :
    iconSize(24, 24),
    margins(4, 2, 4, 2),
	spacingHorizontal(0),
    spacingVertical(2)
{
}

QRect DelegatePrivate::topTextBox(const QStyleOptionViewItem &option,
							 const QModelIndex &index) const
{
    return QFontMetrics(option.font).boundingRect(index.data(Qt::UserRole).toString())
			.adjusted(0, 0, 1, 1);
}

QRect DelegatePrivate::textBox(const QStyleOptionViewItem &option) const
{
	return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}
