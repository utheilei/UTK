#ifndef USTYLEDITEMDELEGATE_H
#define USTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>

class UStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UStyledItemDelegate(QObject *parent = nullptr);
    ~UStyledItemDelegate();

    void setRowHeight(int height);

    int rowHeight();

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;

    void drawChecked(QPainter *painter, const QRect &rect, const QColor &color) const;

private:
    int m_rowHeight = 30;
    int m_width = 18;
};

#endif // USTYLEDITEMDELEGATE_H
