#include "ugraphicsclipeffect.h"

#include <QPainter>
#include <QDebug>
/*!
 * \~chinese \class GraphicsClipEffect
 * \~chinese \brief 用于裁剪窗口的绘制内容
 *
 * \~chinese 支持使用 QPainterPath 设置一个区域，位于区域外的窗口内容将被裁剪,可以通过设置
 * \~chinese \ref margins 控制有效的裁剪区域。如下图所示：
 * \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 * ┏━━━━━━━━━━━━━━┯━━━━━━━━━━━━━━┓
 * ┃              │              ┃
 * ┃              │              ┃
 * ┃     A        m              ┃
 * ┃              │              ┃
 * ┃              │              ┃
 * ┃       ┏┅┅┅┅┅┅┷┅┅┅┅┅┅┓       ┃
 * ┃       ┋B ╭───────╮  ┋       ┃
 * ┃       ┋  │       │  ┋       ┃
 * ┃── m ──┋  │   C   │  ┋── m ──┃
 * ┃       ┋  │       │  ┋       ┃
 * ┃       ┋  ╰───────╯  ┋       ┃
 * ┃       ┗┅┅┅┅┅┅┯┅┅┅┅┅┅┛       ┃
 * ┃              │              ┃
 * ┃              │              ┃
 * ┃              m              ┃
 * ┃              │              ┃
 * ┃              │              ┃
 * ┗━━━━━━━━━━━━━━┷━━━━━━━━━━━━━━┛
 * </pre>
 * \endhtmlonly
 * \~chinese A为被作用的控件区域，B为有效的被裁剪区域，C 为 \ref clipPath 区域，m为
 * \~chinese \ref margins ，则被裁剪掉的区域为： B - C，裁掉的部分将显示
 * \~chinese 下层控件内容，如果下层没有其它控件，将显示主窗口背景。
 *
 * \warning 以这种方式实现对 QWidget 的剪切在性能上会有较大的影响，一般情况下，我们推荐使用 DClipEffectWidget
 * \sa DClipEffectWidget QWidget::setGraphicsEffect
 */

/*!
  * \fn void DGraphicsClipEffect::marginsChanged(QMargins margins)
  * \~chinese 这个信号在 \ref margins 改变时被发送
  */

/*!
  * \fn void DGraphicsClipEffect::clipPathChanged(QPainterPath clipPath)
  * \~chinese 这个信号在 \ref clipPath 改变时被发送
  */


/*!
 * \brief 构造 DGraphicsClipEffect 对象，和普通 QObject 一样，可以传入一个 QObject 对象
 * \~chinese 指针作为其父对象
 * \~chinese \param parent 父对象
 */
UGraphicsClipEffect::UGraphicsClipEffect(QObject* parent)
    : QGraphicsEffect(parent)
{
}

QMargins UGraphicsClipEffect::margins() const
{
    return m_margins;
}

QPainterPath UGraphicsClipEffect::clipPath() const
{
    return m_clipPath;
}

void UGraphicsClipEffect::setMargins(const QMargins &margins)
{
    if (m_margins == margins)
        return;

    m_margins = margins;
    Q_EMIT marginsChanged(margins);
}

void UGraphicsClipEffect::setClipPath(const QPainterPath &clipPath)
{
    if (m_clipPath == clipPath)
        return;

    m_clipPath = clipPath;
    Q_EMIT clipPathChanged(clipPath);
}

void UGraphicsClipEffect::draw(QPainter* painter)
{
    if (!painter->isActive())
    {
        qWarning() << "DGraphicsClipEffect::draw: The painter is not active!";

        drawSource(painter);
        return;
    }

    QPoint offset;
    Qt::CoordinateSystem system = sourceIsPixmap() ? Qt::LogicalCoordinates : Qt::DeviceCoordinates;
    QPixmap pixmap = sourcePixmap(system, &offset, QGraphicsEffect::PadToEffectiveBoundingRect);

    if (pixmap.isNull())
        return;

    QPainter pixmapPainter(&pixmap);
    QPainterPath newPath;

    newPath.addRect(pixmap.rect().marginsRemoved(m_margins));
    newPath -= m_clipPath;
    pixmapPainter.setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    pixmapPainter.setCompositionMode(QPainter::CompositionMode_Clear);
    pixmapPainter.fillPath(newPath, Qt::transparent);

    painter->save();

    if (system == Qt::DeviceCoordinates)
        painter->setWorldTransform(QTransform());

    painter->drawPixmap(offset, pixmap);
    painter->restore();
}
