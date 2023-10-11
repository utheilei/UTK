#include "ustyleditemdelegate.h"
#include "uapplication.h"
#include "style/uproxystyle.h"
#include "style/upalette.h"

#include <QPainter>
#include <QPainterPath>

UStyledItemDelegate::UStyledItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

UStyledItemDelegate::~UStyledItemDelegate()
{
}

void UStyledItemDelegate::setRowHeight(int height)
{
    m_rowHeight = height;
}

int UStyledItemDelegate::rowHeight()
{
    return m_rowHeight;
}

void UStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QRect rect = opt.rect;
    QFontMetrics fm(opt.font);
    QRect textRect = rect;

    QPalette::ColorGroup group = UProxyStyle::colorGroup(&opt, opt.widget);

    UProxyStyle *style = qobject_cast<UProxyStyle*>(uApp->style());
    int radius = style ? style->pixelMetric(UProxyStyle::PM_FrameRadius, &option) : 8;

    // 设置背景色、文本色
    UPalette *palette = uApp->applicationPalette();
    QBrush background;
    QPen foreground = painter->pen();
    foreground.setColor(palette->color(group, QPalette::Text));
    background = palette->color(group, UPalette::ItemBackground);
    bool hasAlternatefeature = opt.features & QStyleOptionViewItem::Alternate;
    if (!hasAlternatefeature)
    {
        background = palette->color(group, UPalette::AlternateBase);
    }
    bool selected = false;
    if (opt.state & QStyle::State_Enabled) {
        if (opt.state & QStyle::State_Selected) {
            selected = true;
            background = palette->color(group, QPalette::Highlight);
            foreground.setColor(palette->color(group, QPalette::HighlightedText));
        } else if (opt.state & QStyle::State_MouseOver) {
            background = palette->color(QPalette::Button);
        }
    }

    painter->fillPath(UProxyStyle::backgroundPath(opt, radius), background);

    QRect iconRect = QRect(rect.left() + 10, rect.top() + 5, 20, 20);
    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    if (!icon.isNull()) {
        textRect.adjust(10 + 20 + 10, 0, 0, 0);
        icon.paint(painter, iconRect, Qt::AlignCenter);
    } else {
        textRect.adjust(10, 0, 0, 0);
    }

    bool isChecked = index.data(Qt::CheckStateRole).toBool();
    if (isChecked) {
        textRect.setWidth(textRect.width() - m_width - 10);
        QRect checkRect = QRect(rect.x() + rect.width() - m_width - 10, rect.y() + (rect.height() - m_width) / 2,
                                m_width, m_width);
        painter->setBrush(selected ? palette->color(group, QPalette::HighlightedText) :
                              palette->color(group, QPalette::Highlight));
        painter->setPen(Qt::NoPen);
        QColor color = selected ? palette->color(group, QPalette::Highlight) :
                           palette->color(group, QPalette::HighlightedText);
        drawChecked(painter, checkRect, color);
    }
    // 绘制文本
    QString text = fm.elidedText(opt.text, Qt::ElideMiddle, textRect.width());
    painter->setFont(opt.font);
    painter->setPen(foreground);
    painter->drawText(textRect, Qt::TextSingleLine | static_cast<int>(opt.displayAlignment), text);
    painter->restore();
}

QSize UStyledItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    return QSize(size.width(), m_rowHeight);
}

void UStyledItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
}

void UStyledItemDelegate::drawChecked(QPainter *painter, const QRect &rect, const QColor &color) const
{
    painter->drawEllipse(rect);
    painter->setPen(QPen(color, 2));
    painter->drawLine(rect.center().x() - 4, rect.center().y(), rect.center().x(), rect.center().y() + 5);
    painter->drawLine(rect.center().x(), rect.center().y() + 5, rect.center().x() + 6, rect.center().y() - 3);
}

