#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

class DelegatePrivate
{
    DelegatePrivate();
    inline QRect topTextBox(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    inline QRect textBox(const QStyleOptionViewItem &option) const;
    QSize iconSize;
    QMargins margins;
    int spacingHorizontal;
    int spacingVertical;
    friend class Delegate;
};

class Delegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit Delegate(QObject *parent = nullptr);
	~Delegate();

	QSize iconSize() const;
	void setIconSize(int width, int height);

	QMargins contentsMargins() const;
	void setContentsMargins(int left, int top, int right, int bottom);

	int horizontalSpacing() const;
	void setHorizontalSpacing(int spacing);

	int verticalSpacing() const;
	void setVerticalSpacing(int spacing);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
	DelegatePrivate *m_ptr;
};

#endif // DELEGATE_H
