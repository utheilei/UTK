#include "delegate.h"
#include "hllistview.h"

#include <QStyle>

#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QPainterPath>

Delegate::Delegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    painter->setRenderHints(painter->renderHints()
                            | QPainter::Antialiasing
                            | QPainter::SmoothPixmapTransform);

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    auto style = opt.widget->style();

    auto palette = opt.widget ? opt.widget->palette() : qApp->palette();

    int borderWidth = 0;
    int borderSpacing = 0;
//    int borderWidth = style->pixelMetric(static_cast<QStyle::PixelMetric>(DStyle::PM_FocusBorderWidth),
//                                         &opt, nullptr);
//    int borderSpacing = style->pixelMetric(static_cast<QStyle::PixelMetric>(DStyle::PM_FocusBorderSpacing),
//                                           &opt, nullptr);
    const QMargins margins(borderWidth + borderSpacing, borderWidth + borderSpacing,
                           borderWidth + borderSpacing, borderWidth + borderSpacing);
    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    QPainterPath path;
    path.addEllipse(opt.rect.marginsRemoved(margins));
    painter->setClipPath(path);

    if(!pixmap.isNull()){
        painter->drawPixmap(opt.rect.marginsRemoved(margins),pixmap);
        painter->setClipping(false);
    } else {
        qreal tw = opt.rect.width() / 3.0;
        qreal th = opt.rect.height() / 3.0;

        //绘制背景
        //QRectF rect(tw + opt.rect.x(), th + opt.rect.y(), tw, th);
        //rect.moveCenter(QRect(opt.rect).center());
        painter->setPen(Qt::NoPen);
        painter->setBrush(palette.color(QPalette::Button));
        painter->drawEllipse(opt.rect.marginsRemoved(margins));

        //画+号
        qreal x1 = opt.rect.x() + tw ;
        qreal y1 = opt.rect.y() + opt.rect.height() / 2.0 - 0.5;
        qreal x2 = opt.rect.x() + opt.rect.width() / 2.0 - 0.5;
        qreal y2 = opt.rect.y() + th;
        painter->setBrush(palette.color(QPalette::Text));
        painter->drawRect(QRectF(x1, y1, tw, 1.0));
        painter->drawRect(QRectF(x2, y2, 1.0, th));
    }

    //index.data(Qt::CheckStateRole) == Qt::Checked  (opt.state & QStyle::State_Selected)
    //opt.state.testFlag(QStyle::State_Selected)
    if ((opt.state & QStyle::State_Selected) && !pixmap.isNull()) {
        painter->setPen(QPen(opt.palette.highlight(), borderWidth));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(opt.rect.adjusted(1, 1, -1, -1));

        //在中间绘制选中小图标
        int radius = 8;
        int cx = opt.rect.marginsRemoved(margins).center().x();
        int cy = opt.rect.marginsRemoved(margins).center().y();
        QRect crect(QPoint(cx - radius, cy - radius), QPoint(cx + radius, cy + radius));
        opt.rect = crect;
        opt.state |= QStyle::State_On;
        style->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &opt, painter, nullptr);
    }
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    return index.data(Qt::SizeHintRole).toSize();
}

StyledDelegate::StyledDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void StyledDelegate::setItemSpace(int space)
{
    if (itemSpace != space)
        itemSpace = space;
}

void StyledDelegate::setIconSize(const QSize &size)
{
    if (m_size != size)
        m_size = size;
}

void StyledDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    painter->setRenderHints(painter->renderHints()
                            | QPainter::Antialiasing
                            | QPainter::SmoothPixmapTransform);

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QListView *view = qobject_cast<const QListView*>(opt.widget);
    if (view) {
        if (view->flow() == QListView::LeftToRight) {
            opt.rect.adjust(0, 0, 0 - itemSpace, 0);
        } else {
            opt.rect.adjust(0, 0, 0, 0 - itemSpace);
        }
    }

    QPalette::ColorGroup group;

    if (!(opt.state & QStyle::State_Enabled))
        group = QPalette::Disabled;
    else
        group = !opt.widget->isActiveWindow() ? QPalette::Inactive : QPalette::Active;

    int borderWidth = 0;
    int borderSpacing = 0;
    const QMargins margins(borderWidth + borderSpacing, borderWidth + borderSpacing,
                           borderWidth + borderSpacing, borderWidth + borderSpacing);

    int width = qMin(opt.rect.width(),opt.rect.height())/4;
    const QMargins margin(width, width, width, width);

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();

    QBrush background = opt.palette.brush(group, QPalette::AlternateBase);
    QPen foreground = opt.palette.color(group, QPalette::Text);

    if (opt.state & QStyle::State_Enabled) {
        if (opt.state & QStyle::State_Selected) {
            background = opt.palette.brush(group, QPalette::Highlight);
            foreground.setColor(opt.palette.color(group, QPalette::HighlightedText));
        } else if (opt.state & QStyle::State_MouseOver) {
            background = opt.palette.brush(group, QPalette::Button);
        }
    }

    QPainterPath path;
    path.addRoundedRect(opt.rect, radius, radius);
    painter->setClipPath(path);
    painter->fillPath(path, background);

    QSize iconSize = QSize();
    if(!icon.isNull()) {
        iconSize = m_size;
        drawIcon(painter, opt, icon, iconSize);
    }

    QRect textRect = opt.rect;
    textRect.setX(opt.rect.x() + 10*2 + iconSize.width());
    painter->setFont(opt.font);
    painter->setPen(foreground);
    painter->drawText(textRect, Qt::TextSingleLine | Qt::AlignVCenter, opt.text);
}

QSize StyledDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    return index.data(Qt::SizeHintRole).toSize();
}

void StyledDelegate::drawIcon(QPainter *painter, const QStyleOptionViewItem &option, const QIcon &icon, const QSize &size) const
{
    QIcon::Mode iconMode = QIcon::Normal;
    QIcon::State iconState = QIcon::Off;

    if (option.state & QStyle::State_Enabled) {
        if (option.state & QStyle::State_Selected) {
            iconMode = QIcon::Selected;
        } else if (option.state & QStyle::State_Active) {
            iconMode = QIcon::Active;
        }
    } else {
        iconMode = QIcon::Disabled;
    }

    if (option.state & QStyle::State_On) {
        iconState = QIcon::On;
    }

    QRect iconRect = QRect(QPoint(option.rect.x() + 10, option.rect.y() + (option.rect.height() - size.height())/2), size);
    icon.paint(painter, iconRect, Qt::AlignCenter, iconMode, iconState);
}

TiTleStyledDelegate::TiTleStyledDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void TiTleStyledDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    painter->setRenderHints(painter->renderHints()
                            | QPainter::Antialiasing
                            | QPainter::SmoothPixmapTransform);

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int borderWidth = 0;
    int borderSpacing = 0;
    const QMargins margins(borderWidth + borderSpacing, borderWidth + borderSpacing,
                           borderWidth + borderSpacing, borderWidth + borderSpacing);

    int width = qMin(opt.rect.width(),opt.rect.height())/4;
    const QMargins margin(width, width, width, width);

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();

    QBrush m_brush = opt.palette.brush(QPalette::AlternateBase);

    if (opt.state & QStyle::State_Enabled) {
        if (opt.state & QStyle::State_MouseOver) {
            m_brush = opt.palette.brush(QPalette::Button);
        }
    }

    QPainterPath path;
    path.addRoundedRect(opt.rect, radius, radius);
    painter->setClipPath(path);
    painter->fillPath(path, m_brush);

    if(!icon.isNull()){
        icon.paint(painter,opt.rect.marginsRemoved(margin),Qt::AlignCenter);
    }

    QRect textRect = opt.rect;
    textRect.setY(opt.rect.y() + opt.rect.height()*5/6);
    painter->drawText(textRect, Qt::TextSingleLine | Qt::AlignHCenter, opt.text);
}

QSize TiTleStyledDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    return index.data(Qt::SizeHintRole).toSize();
}
