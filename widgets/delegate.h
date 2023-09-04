#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Delegate(QObject *parent = nullptr);

    // painting
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // set item size
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class StyledDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StyledDelegate(QObject *parent = nullptr);

    // painting
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // set item size
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setItemSpace(int space);

    void setIconSize(const QSize &size);

private:
    void drawIcon(QPainter *painter, const QStyleOptionViewItem &option, const QIcon &icon, const QSize &size) const;

private:
    int radius = 8;
    QSize m_size = QSize(26, 26);
    int itemSpace = 0;
};

class TiTleStyledDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TiTleStyledDelegate(QObject *parent = nullptr);

    // painting
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // set item size
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int radius = 20;
};

#endif // DELEGATE_H
