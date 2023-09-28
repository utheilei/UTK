#include "ArrowCircleButton.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

ArrowCircleButton::ArrowCircleButton(QWidget *parent)
    : QAbstractButton(parent)
{
    setFixedSize(PIX_SIZE, PIX_SIZE);
    setCheckable(true);
}

void ArrowCircleButton::setSelected(const bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
        update();
    }
}

void ArrowCircleButton::setArrowed(const bool arrowed)
{
    if (m_arrowed != arrowed) {
        m_arrowed = arrowed;
        update();
    }
}

void ArrowCircleButton::paintEvent(QPaintEvent *e)
{
    QPainterPath painterPath;
    painterPath.addEllipse(QRect(0, 0, width(), height()));

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setClipPath(painterPath);
    painter.drawPixmap(e->rect(), icon().pixmap(QSize(PIX_SIZE, PIX_SIZE)));

    if (m_selected) {
        QPen pen(Qt::transparent);
        pen.setWidth(4);
        pen.setColor(Qt::green);
        painter.setPen(pen);
        painter.setBrush(Qt::transparent);
        painter.drawEllipse(rect());
    };

    //当鼠标移动到图像上面
    if (m_hover) {
        painter.setPen(Qt::NoPen);
        //宽高
        int w = this->rect().width();
        int h = this->rect().height();
        //矩形
        QRect rect(0, h - h / 4, w, h - h / 4);
        // 反走样
        painter.setRenderHint(QPainter::Antialiasing, true);
        // 设置渐变色
        QLinearGradient linear(QPoint(0, h - h / 4), QPoint(0, h));
        linear.setColorAt(0, QColor(0, 0, 0, 0.00 * 255));
        linear.setColorAt(1, QColor(0, 0, 0, 0.50 * 255));

        // 设置显示模式
        linear.setSpread(QGradient::PadSpread);
        painter.setBrush(linear);
        painter.drawRect(rect);
    }

    if (!m_arrowed) {
        QPen pen(Qt::transparent);
        pen.setWidth(2);
        pen.setColor(Qt::white);
        painter.setPen(pen);
        //把直径平均分成10份
        int portion = this->rect().width() / 10;
        //圆中心点坐标
        QPoint cpt = this->rect().center();
        //绘制左边直线
        painter.drawLine(QPoint(cpt.x() - portion / 2, cpt.y() + portion * 4 - portion / 2),
                         QPoint(cpt.x(), cpt.y() + portion * 4));
        //绘制右边直线
        painter.drawLine(QPoint(cpt.x() + portion / 2, cpt.y() + portion * 4 - portion / 2),
                         QPoint(cpt.x(), cpt.y() + portion * 4));
    } else {
        QPen pen(Qt::transparent);
        pen.setWidth(2);
        pen.setColor(Qt::white);
        painter.setPen(pen);
        //把直径平均分成10份
        int portion = this->rect().width() / 10;
        //圆中心点坐标
        QPoint cpt = this->rect().center();
        //绘制左边直线
        painter.drawLine(QPoint(cpt.x() - portion / 2, cpt.y() + portion * 4),
                         QPoint(cpt.x(), cpt.y() + portion * 4 - portion / 2));
        //绘制右边直线
        painter.drawLine(QPoint(cpt.x() + portion / 2, cpt.y() + portion * 4),
                         QPoint(cpt.x(), cpt.y() + portion * 4 - portion / 2));
    }
}

void ArrowCircleButton::enterEvent(QEvent *)
{
    m_hover = true;
    update();
}

void ArrowCircleButton::leaveEvent(QEvent *)
{
    m_hover = false;
    update();
}
