#include "uproxystyle.h"
#include "upalette.h"

#include <QDebug>
#include <QPixmap>
#include <QPixmapCache>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionMenuItem>
#include <QPainterPath>

namespace
{
    const QString SUB_MENU_ICON = ":/images/sub_menu.svg";
    const int FRAME_BORDER_RADIUS = 8;
    const int ICON_PIXEL_SIZE = 16;
    const int ICON_LEFT_MARGIN = 8;
    const int MARGINS = 4;
    const int LEFT_MARGIN = ICON_PIXEL_SIZE + ICON_LEFT_MARGIN * 2;
}  // namespace

UProxyStyle::UProxyStyle()
    : QProxyStyle()
{
}

UProxyStyle::~UProxyStyle()
{
}

QPalette UProxyStyle::standardPalette() const
{
    return UPalette();
}

int UProxyStyle::styleHint(QStyle::StyleHint hint,
                           const QStyleOption* option,
                           const QWidget* widget,
                           QStyleHintReturn* returnData) const
{
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

int UProxyStyle::pixelMetric(QStyle::PixelMetric metric,
                             const QStyleOption* option,
                             const QWidget* widget) const
{
    switch (metric)
    {
        case QStyle::PM_MenuScrollerHeight:
            return 15;
        case QStyle::PM_SubMenuOverlap:
            return 0;
        case QStyle::PM_MenuHMargin:
            return 2;
        case QStyle::PM_MenuVMargin:
            return 8;
        case QStyle::PM_MenuPanelWidth:
            return 0;
        case QStyle::PM_MenuTearoffHeight:
            return 0;
        case QStyle::PM_MenuDesktopFrameWidth:
            return 0;
        case QStyle::PM_SliderLength:
            return 30;
        case PM_ScrollBarExtent:
            return 8;
        default:
            return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

void UProxyStyle::drawControl(QStyle::ControlElement element,
                              const QStyleOption* option,
                              QPainter* painter,
                              const QWidget* widget) const
{
    if (element > QStyle::CE_CustomBase)
    {
        drawControl(static_cast<UControlElement>(element), option, painter, widget);
        return;
    }
    switch (element)
    {
        case CE_MenuTearoff:
            break;
        case CE_MenuEmptyArea: //空白区域
            break;
        case CE_MenuItem: //画菜单项
            if (const QStyleOptionMenuItem* menuItem = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
            {
                painter->save();

                bool hover = menuItem->state & State_Selected && menuItem->state & State_Enabled;
                if (hover)   //鼠标滑过，先画一个矩形，使后面的文字不会被覆盖
                {
                    painter->setBrush(menuItem->palette.brush(colorGroup(menuItem->state), QPalette::Button));
                    painter->drawRect(menuItem->menuRect.adjusted(MARGINS, 0, -MARGINS, 0));
                }

                if (menuItem->menuItemType == QStyleOptionMenuItem::Separator)  //画分隔线
                {
                    drawSeparator(menuItem, painter);
                }
                else if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu)   //画子菜单
                {
                    drawSubMenuItem(menuItem, painter);
                }
                else     //文字菜单项
                {
                    drawMenuItem(menuItem, painter);
                }

                painter->restore();
            }
            break;
        default:
            QProxyStyle::drawControl(element, option, painter, widget);
            break;
    }
}

void UProxyStyle::drawControl(UControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{

}

void UProxyStyle::drawPrimitive(QStyle::PrimitiveElement element,
                                const QStyleOption* option,
                                QPainter* painter,
                                const QWidget* widget) const
{
    if (element > QStyle::PE_CustomBase)
    {
        drawPrimitive(static_cast<UPrimitiveElement>(element), option, painter, widget);
        return;
    }
    switch (element)
    {
        case QStyle::PE_FrameMenu: //整个菜单widget的边框色
        {
            break;
        }
        case QStyle::PE_PanelMenu: //整个菜单widget的背景色
        {
            painter->setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            QRect rect = option->rect.adjusted(MARGINS, MARGINS, -MARGINS, -MARGINS);
            path.addRoundedRect(rect, FRAME_BORDER_RADIUS, FRAME_BORDER_RADIUS);
            QStyle::State widgetState;
            if (widget && widget->isActiveWindow())
                widgetState |= QStyle::State_Active;
            if (widget && widget->isEnabled())
                widgetState |= QStyle::State_Enabled;
            painter->fillPath(path, option->palette.brush(colorGroup(widgetState), QPalette::Base));

            QColor color(92, 93, 95, 50);
            int arr[MARGINS] = {10, 20, 30, 40};
            for (int i = 0; i < MARGINS; i++)
            {
                QPainterPath path;
                path.setFillRule(Qt::WindingFill);
                QRect rect = option->rect.adjusted(i, i, -i, -i);
                path.addRoundedRect(rect, FRAME_BORDER_RADIUS, FRAME_BORDER_RADIUS);

                color.setAlpha(arr[i]);
                painter->setPen(color);
                painter->drawPath(path);
            }
            break;
        }
        default:
            QProxyStyle::drawPrimitive(element, option, painter, widget);
            break;
    }
}

void UProxyStyle::drawPrimitive(UPrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{

}

void UProxyStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    if (control > QStyle::CC_CustomBase)
    {
        drawComplexControl(static_cast<UComplexControl>(control), option, painter, widget);
        return;
    }


    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void UProxyStyle::drawComplexControl(UComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
        case CC_USlider:
            if (const QStyleOptionSlider* styleOptionSlider = qstyleoption_cast<const QStyleOptionSlider*>(option))
            {
                QRect grooveRect = subControlRect(CC_Slider, styleOptionSlider, SC_SliderGroove, widget);
                grooveRect = QRect(grooveRect.x(), grooveRect.y() + grooveRect.height() / 2 - 5, grooveRect.width(), 10);
                QLinearGradient gradient(0, 0, grooveRect.width(), grooveRect.height());//y轴使用渐变
                gradient.setColorAt(0.0, QColor("#0fd850"));
                gradient.setColorAt(1.0, QColor("#f9f047"));
                painter->setPen(Qt::transparent);
                painter->setBrush(gradient);

                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->drawRoundedRect(grooveRect, 8, 8);

                QRect handleRect = subControlRect(CC_Slider, styleOptionSlider, SC_SliderHandle, widget);
                int width = qMin(handleRect.width(), handleRect.height());
                handleRect = QRect(handleRect.x(), handleRect.y() + (handleRect.height() - width) / 2, width, width);
                painter->setBrush(Qt::blue);
                painter->drawEllipse(handleRect);
            }
            break;
        case CC_UScrollBar:
            if (const QStyleOptionSlider* scrollbar = qstyleoption_cast<const QStyleOptionSlider*>(option))
            {
                qreal opacity = painter->opacity();
                painter->setRenderHint(QPainter::Antialiasing);
                painter->setOpacity(1.0);
                painter->fillRect(scrollbar->rect, option->palette.brush(QPalette::Window));
                painter->setBrush(Qt::transparent);
                painter->setPen(Qt::NoPen);

                QPainterPath path;
                path.addRoundedRect(scrollbar->rect, 4, 4);
                painter->setClipPath(path);
                painter->drawRect(scrollbar->rect);

                if (scrollbar->subControls & SC_ScrollBarSlider)
                {
                    QRect sliderRect = proxy()->subControlRect(CC_ScrollBar, scrollbar, SC_ScrollBarSlider, widget);
                    if (sliderRect.isValid())
                    {
                        QColor sliderColor = QColor(193, 193, 193);
                        if ((scrollbar->activeSubControls & SC_ScrollBarSlider))
                        {
                            if (scrollbar->state & State_MouseOver)
                            {
                                sliderColor = sliderColor.darker(120);
                            }
                            if (scrollbar->state & State_Sunken)
                            {
                                sliderColor = sliderColor.darker(140);
                            }
                        }
                        painter->setOpacity(opacity);
                        painter->setBrush(sliderColor);
                        painter->drawRoundedRect(sliderRect, 4, 4);
                    }
                }
            }
            break;
        default:
            break;
    }
}

QSize UProxyStyle::sizeFromContents(QStyle::ContentsType type,
                                    const QStyleOption* option,
                                    const QSize &contentsSize,
                                    const QWidget* widget) const
{
    QSize originSize = QProxyStyle::sizeFromContents(type, option, contentsSize,
                       widget);
    return originSize;
}

void UProxyStyle::drawMenuItem(const QStyleOptionMenuItem* menuItem,
                               QPainter* painter) const
{
    drawMenuItemIcon(menuItem, painter);
    painter->setPen(menuItem->palette.color(colorGroup(menuItem->state), QPalette::Text));
    QRect textRect = menuItem->rect;
    textRect = textRect.adjusted(MARGINS, 0, -MARGINS, 0);
    QFont font = menuItem->font;
    painter->setFont(font);
    int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
    QString title;
    QString shortcutText;
    if (menuItem->text.contains(QLatin1Char('\t')))
    {
        QStringList list = menuItem->text.split(QLatin1Char('\t'));
        if (list.size() > 1)
            shortcutText = list.at(1);
        title = list.at(0);
    }
    else
    {
        title = menuItem->text;
    }

    if (!shortcutText.isEmpty())  //画快捷键文字
    {
        int x, y, w, h;
        menuItem->rect.getRect(&x, &y, &w, &h);
        int dim = (h - 2 * 2) / 2;
        int xpos = x + w - 2 - 1 - dim;
        QRect vSubMenuRect = visualRect(menuItem->direction, menuItem->rect, QRect(xpos, y + h / 2 - dim / 2, dim, dim));
        int fw = menuItem->fontMetrics.width(shortcutText);
        textRect.moveLeft(menuItem->rect.right() - fw - vSubMenuRect.width());
        textRect = visualRect(menuItem->direction, menuItem->rect, textRect);
        painter->drawText(textRect, text_flags, shortcutText);
    }

    textRect.moveLeft(LEFT_MARGIN);
    textRect = visualRect(menuItem->direction, menuItem->rect, textRect);
    painter->drawText(textRect, text_flags, title);
}

void UProxyStyle::drawSeparator(const QStyleOptionMenuItem* menuItem,
                                QPainter* painter) const
{
    QRect rect = menuItem->rect.adjusted(MARGINS, 0, -MARGINS, 0);
    painter->setPen(QPen(Qt::black));
    painter->drawLine(rect.left(), rect.center().y(), rect.right(), rect.center().y());
}

void UProxyStyle::drawMenuItemIcon(const QStyleOptionMenuItem* menuItem,
                                   QPainter* painter) const
{
    QPixmap pixmap = getIconPixmap(menuItem->state,
                                   menuItem->icon,
                                   QSize(ICON_PIXEL_SIZE,
                                         ICON_PIXEL_SIZE));
    if (pixmap.isNull())
    {
        return;
    }

    int y = menuItem->rect.y() + (menuItem->rect.height() - pixmap.height()) / 2;
    painter->drawPixmap(ICON_LEFT_MARGIN + MARGINS, y, pixmap.width(), pixmap.height(), pixmap);
}

void UProxyStyle::drawSubMenuItem(const QStyleOptionMenuItem* menuItem,
                                  QPainter* painter) const
{
    drawMenuItemIcon(menuItem, painter);
    QRect textRect = menuItem->rect;
    textRect = textRect.adjusted(MARGINS, 0, -MARGINS, 0);
    textRect.moveLeft(LEFT_MARGIN);
    painter->setPen(menuItem->palette.text().color());
    painter->drawText(textRect, Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextSingleLine | Qt::TextDontClip, menuItem->text);
    int x, y, w, h;
    menuItem->rect.getRect(&x, &y, &w, &h);
    int dim = (h - 2 * 2) / 2;
    int xpos = x + w - 2 - 1 - dim;
    QRect vSubMenuRect = visualRect(menuItem->direction, menuItem->rect, QRect(xpos, y + h / 2 - dim / 2, dim, dim));
    QPixmap pixmap(getSubMenuPixmap(menuItem->state));
    painter->drawPixmap(vSubMenuRect, pixmap);
}

void UProxyStyle::drawMenuItemCheckedIcon(const QStyleOptionMenuItem* menuItem,
        QPainter* painter) const
{
    Q_UNUSED(menuItem)
    Q_UNUSED(painter)
    //    if (menuItem->checked) {
    //        QPixmap pixmap;
    //        QPixmapCache::find(kCheckedIconCacheKey, &pixmap);

    //        int height = menuItem->rect.height();
    //        int x = menuItem->rect.width() - TEXT_LEFT_MARGIN - ICON_PIXEL_SIZE;
    //        int y = menuItem->rect.y() + (height - pixmap.height()) / 2;
    //        painter->drawPixmap(x, y, pixmap.width(), pixmap.height(), pixmap);
    //    }
}

QPixmap UProxyStyle::getSubMenuPixmap(const QStyle::State state) const
{
    if (state & QStyle::State_Selected)
    {
        return SUB_MENU_ICON;
    }
    else
    {
        return SUB_MENU_ICON;
    }
}

QPixmap UProxyStyle::getIconPixmap(const QStyle::State state,
                                   const QIcon &icon,
                                   const QSize &size) const
{
    if (!icon.isNull())
    {
        QIcon::Mode iconMode = QIcon::Normal;
        QIcon::State iconState = QIcon::Off;

        if (state & QStyle::State_Enabled)
        {
            if (state & QStyle::State_Selected)
            {
                iconMode = QIcon::Selected;
            }
            else if (state & QStyle::State_Active)
            {
                iconMode = QIcon::Active;
            }
        }
        else
        {
            iconMode = QIcon::Disabled;
        }

        if (state & QStyle::State_On)
        {
            iconState = QIcon::On;
        }

        return icon.pixmap(size, iconMode, iconState);
    }
    else
    {
        return QPixmap();
    }
}

QPalette::ColorGroup UProxyStyle::colorGroup(const QStyle::State state) const
{
    QPalette::ColorGroup group;

    if (!(state & QStyle::State_Enabled))
        group = QPalette::Disabled;
    else
        group = QPalette::Active;

    return group;
}
