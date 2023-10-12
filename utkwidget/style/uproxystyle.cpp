#include "uproxystyle.h"
#include "upalette.h"

#include <qdrawutil.h>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionMenuItem>
#include <QPainterPath>
#include <QApplication>

namespace
{
    const int FRAME_BORDER_RADIUS = 2;
    const int ICON_PIXEL_SIZE = 16;
    const int ICON_LEFT_MARGIN = 8;
    const int MARGINS = 0;
    const int LEFT_MARGIN = ICON_PIXEL_SIZE + ICON_LEFT_MARGIN * 2;
}  // namespace

UProxyStyle::UProxyStyle()
    : QProxyStyle()
{
}

UProxyStyle::~UProxyStyle()
{
}

UTheme::ThemeType UProxyStyle::theme() const
{
    return m_theme.theme();
}

void UProxyStyle::setTheme(const UTheme::ThemeType &type)
{
    m_theme.setTheme(type);
    QString themeName(m_theme.themeName());
    QIcon::setThemeName(themeName);
    QIcon::setFallbackThemeName(themeName);
    emit themeChanged(type);
}

QPalette UProxyStyle::standardPalette() const
{
    return *m_theme.palette();
}

UPalette* UProxyStyle::palette() const
{
    return m_theme.palette();
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
    if (metric > QStyle::PM_CustomBase)
    {
        return pixelMetric(static_cast<UPixelMetric>(metric), option, widget);
    }
    switch (metric)
    {
        case QStyle::PM_MenuScrollerHeight:
            return 15;
        case QStyle::PM_SubMenuOverlap:
            return 0;
        case QStyle::PM_MenuHMargin:
            return 2;
        case QStyle::PM_MenuVMargin:
            return 4;
        case QStyle::PM_MenuPanelWidth:
            return 1;
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

int UProxyStyle::pixelMetric(UPixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
        case UProxyStyle::PM_FocusBorderWidth:
            return 1;
        case UProxyStyle::PM_FocusBorderSpacing:
            return 6;
        case UProxyStyle::PM_FrameRadius:
            return 8;
        case UProxyStyle::PM_ShadowRadius:
            return 10;
        case UProxyStyle::PM_FrameMargins:
            return 9;
        default:
            break;
    }

    return -1;
}

void UProxyStyle::drawControl(QStyle::ControlElement element, const QStyleOption* option,
                              QPainter* painter, const QWidget* widget) const
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
                bool hover = (menuItem->state & State_Selected) && (menuItem->state & State_Enabled);
                if (hover)   //鼠标滑过，先画一个矩形，使后面的文字不会被覆盖
                {
                    painter->setBrush(menuItem->palette.brush(colorGroup(menuItem->state), QPalette::Highlight));
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
                    if (menuItem->checked)
                    {
                        QRect textRect = menuItem->rect;
                        textRect.moveLeft(ICON_LEFT_MARGIN);
                        textRect = visualRect(menuItem->direction, menuItem->rect, textRect);
                        painter->setRenderHint(QPainter::Antialiasing);
                        painter->setPen(QPen(palette()->color(QPalette::Text), 2));
                        painter->drawLine(textRect.x(), textRect.y() + textRect.height() / 2, textRect.x() + 5, textRect.y() + textRect.height() / 2 + 5);
                        painter->drawLine(textRect.x() + 5, textRect.y() + textRect.height() / 2 + 5, textRect.x() + 13, textRect.y() + textRect.height() / 2 - 4);
                    }
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
    switch (element)
    {
        case CE_RadioButton:
        case CE_CheckBox:
            if (const QStyleOptionButton* btn = qstyleoption_cast<const QStyleOptionButton*>(option))
            {
                bool isRadio = (element == CE_RadioButton);
                QStyleOptionButton subopt = *btn;
                subopt.rect = subElementRect(isRadio ? SE_RadioButtonIndicator
                                             : SE_CheckBoxIndicator, btn, widget);
                subopt.rect = subopt.rect.adjusted(1, 0, 0, 0);
                drawPrimitive(isRadio ? PE_IndicatorRadioButton : PE_IndicatorCheckBox,
                              &subopt, painter, widget);
                subopt.rect = subElementRect(isRadio ? SE_RadioButtonContents
                                             : SE_CheckBoxContents, btn, widget);
                subopt.rect = subopt.rect.adjusted(1, 0, 0, 0);
                uint alignment = visualAlignment(subopt.direction, Qt::AlignLeft | Qt::AlignVCenter);
                QPixmap pix;
                QRect textRect = subopt.rect;
                if (!subopt.icon.isNull())
                {
                    QWindow* window = widget ? widget->window()->windowHandle() : nullptr;
                    pix = subopt.icon.pixmap(window, subopt.iconSize, subopt.state & State_Enabled ? QIcon::Normal : QIcon::Disabled);
                    drawItemPixmap(painter, subopt.rect, alignment, pix);
                    if (subopt.direction == Qt::RightToLeft)
                        textRect.setRight(textRect.right() - subopt.iconSize.width() - 4);
                    else
                        textRect.setLeft(textRect.left() + subopt.iconSize.width() + 4);
                }
                if (!subopt.text.isEmpty())
                {
                    drawItemText(painter, textRect, alignment | Qt::TextShowMnemonic,
                                 subopt.palette, subopt.state & State_Enabled, subopt.text, QPalette::WindowText);
                }
            }
            break;
        default:
            break;
    }
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
            qDrawShadeRect(painter, option->rect, option->palette, option->state & State_Sunken, 1);
            break;
        }
        case QStyle::PE_PanelMenu: //整个菜单widget的背景色
        {
            painter->setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addRoundedRect(option->rect, FRAME_BORDER_RADIUS, FRAME_BORDER_RADIUS);
            painter->setClipPath(path);
            QStyle::State widgetState;
            if (widget && widget->isActiveWindow())
                widgetState |= QStyle::State_Active;
            if (widget && widget->isEnabled())
                widgetState |= QStyle::State_Enabled;
            painter->fillPath(path, option->palette.brush(QPalette::Base));
            break;
        }
        case PE_PanelItemViewRow:
        {
            if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                QPalette::ColorGroup cg = colorGroup(vopt, widget);
                if ((vopt->state & QStyle::State_Selected) &&
                    proxy()->styleHint(QStyle::SH_ItemView_ShowDecorationSelected, option, widget))
                    painter->fillPath(backgroundPath(*vopt, 8), vopt->palette.brush(cg, QPalette::Highlight));
                else if (vopt->features & QStyleOptionViewItem::Alternate)
                    painter->fillPath(backgroundPath(*vopt, 8), vopt->palette.brush(cg, QPalette::AlternateBase));
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
    switch (element)
    {
        case PE_IndicatorCheckBox:
        {
            QRect rect = option->rect;
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(option->palette.brush(QPalette::Button));
            painter->drawRoundedRect(rect, 2, 2);
            if (option->state & State_On)
            {
                painter->setPen(QPen(option->palette.brush(QPalette::WindowText), 2));
                painter->drawLine(rect.x() + 3, rect.center().y(), rect.center().x(), rect.y() + rect.height() - 4);
                painter->drawLine(rect.center().x(), rect.y() + rect.height() - 4, rect.x() + rect.width() - 3, rect.y() + rect.height() / 4);
            }
            else if (option->state & State_NoChange)
            {
                painter->setPen(QPen(option->palette.brush(QPalette::WindowText), 2));
                painter->drawLine(rect.x() + 3, rect.center().y(), rect.x() + rect.width() - 3, rect.center().y());
            }
            QColor color;
            if (option->state & State_Sunken)
            {
                color = option->palette.color(QPalette::Highlight);
            }
            else if (option->state & State_MouseOver)
            {
                color = option->palette.color(QPalette::WindowText);
            }
            if (color.isValid())
            {
                painter->setPen(QPen(color, 1));
                painter->setBrush(Qt::NoBrush);
                painter->drawRoundedRect(rect, 2, 2);
            }
            break;
        }
        case PE_IndicatorRadioButton:
        {
            QRectF rect = QRectF(0.5, 0.5, option->rect.width(), option->rect.width());
            painter->setRenderHints(QPainter::Antialiasing);
            QColor color = option->palette.color(QPalette::WindowText);
            if (option->state & State_Sunken)
            {
                color = option->palette.color(QPalette::Highlight).darker(120);
            }
            else if (option->state & State_MouseOver)
            {
                color = option->palette.color(QPalette::Highlight);
            }
            if (option->state & State_On)
            {
                painter->setBrush(color);
                painter->setPen(Qt::NoPen);
                painter->drawEllipse(rect.adjusted(2, 2, -2, -2));
            }
            else if (option->state & State_NoChange)
            {
                painter->setPen(QPen(color, 2));
                painter->drawLine(QPointF(rect.x() + 3, rect.center().y()), QPointF(rect.x() + rect.width() - 3, rect.center().y()));
            }
            painter->setPen(QPen(color));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(rect);
            break;
        }
        default:
            break;
    }
}

void UProxyStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    if (control > QStyle::CC_CustomBase)
    {
        drawComplexControl(static_cast<UComplexControl>(control), option, painter, widget);
        return;
    }
    switch (control)
    {
        case CC_Slider:
            if (const QStyleOptionSlider* styleOptionSlider = qstyleoption_cast<const QStyleOptionSlider*>(option))
            {
                QRect grooveRect = subControlRect(CC_Slider, styleOptionSlider, SC_SliderGroove, widget);
                grooveRect = QRect(grooveRect.x(), grooveRect.y() + grooveRect.height() / 2 - 5, grooveRect.width(), 10);
                painter->setPen(Qt::NoPen);
                painter->setBrush(styleOptionSlider->palette.brush(QPalette::Button));

                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->drawRoundedRect(grooveRect, 4, 4);

                QRect handleRect = subControlRect(CC_Slider, styleOptionSlider, SC_SliderHandle, widget);
                int width = qMin(handleRect.width(), handleRect.height());
                handleRect = QRect(handleRect.x(), handleRect.y() + (handleRect.height() - width) / 2, width, width);
                painter->setPen(Qt::gray);
                painter->setBrush(Qt::white);
                painter->drawEllipse(handleRect);
            }
            break;
        default:
            QProxyStyle::drawComplexControl(control, option, painter, widget);
            break;
    }
}

void UProxyStyle::drawComplexControl(UComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
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

QPalette::ColorGroup UProxyStyle::colorGroup(const QStyleOption* option, const QWidget* widget)
{
    bool isEnabled = widget ? widget->isEnabled() : (option->state & QStyle::State_Enabled);
    QPalette::ColorGroup group;
    if (!isEnabled)
        group = QPalette::Disabled;
    else
        group = !QApplication::activeWindow() ? QPalette::Inactive : QPalette::Active;

    return group;
}

QPainterPath UProxyStyle::backgroundPath(const QStyleOptionViewItem &option, int radius)
{
    QPainterPath path;
    QRect rect = option.rect;
    switch (option.viewItemPosition)
    {
        case QStyleOptionViewItem::ViewItemPosition::Beginning:
        {
            path.addRoundedRect(rect, radius, radius);
            rect.setX(rect.width() / 2);
            QPainterPath addPath;
            addPath.addRect(rect);
            path = path.united(addPath);
            break;
        }
        case QStyleOptionViewItem::ViewItemPosition::Middle:
        {
            path.addRect(rect);
            break;
        }
        case QStyleOptionViewItem::ViewItemPosition::End:
        {
            path.addRoundedRect(rect, radius, radius);
            rect.setWidth(rect.width() / 2);
            QPainterPath addPath;
            addPath.addRect(rect);
            path = path.united(addPath);
            break;
        }
        case QStyleOptionViewItem::ViewItemPosition::OnlyOne:
        {
            path.addRoundedRect(rect, radius, radius);
            break;
        }
        default:
            break;
    }

    return path;
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
        int fw = menuItem->fontMetrics.horizontalAdvance(shortcutText);
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
    int x = ICON_LEFT_MARGIN + MARGINS;
    int y = menuItem->rect.y() + (menuItem->rect.height() - pixmap.height()) / 2;
    painter->drawPixmap(x, y, pixmap.width(), pixmap.height(), pixmap);
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

QPixmap UProxyStyle::getSubMenuPixmap(const QStyle::State state) const
{
    if (state & QStyle::State_Selected)
    {
        return QIcon::fromTheme("sub_menu").pixmap(10, 10);
    }
    else
    {
        return QIcon::fromTheme("sub_menu").pixmap(10, 10);
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
