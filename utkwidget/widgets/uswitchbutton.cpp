#include "uswitchbutton.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

class USwitchButtonPrivate
{
    Q_DECLARE_PUBLIC(USwitchButton)

public:
    USwitchButtonPrivate(USwitchButton* q) : q_ptr(q) {}
    ~USwitchButtonPrivate() {};

    void init(bool bEnergtic);

private:
    int  m_offset = 0;
    bool m_hovering = false;
    qreal m_margin = 2;
    qreal m_lineWidth = 7;
    bool   m_isTristate = false;
    Qt::CheckState  m_ckstate = Qt::Unchecked;
    Qt::CheckState  m_lastState = Qt::Unchecked;
    QString m_checkedText = "";
    QString m_noCheckedText = "";
    qreal  m_thumbWidth = 12;
    int    m_duration = 120;
    USwitchButton* q_ptr = nullptr;
};

void USwitchButtonPrivate::init(bool bEnergtic)
{
    Q_Q(USwitchButton);
    q->setCheckable(true);
    q->setMouseTracking(true);
    q->setForegroundRole(QPalette::WindowText);
    q->setAttribute(Qt::WA_MacShowFocusRect);
    // set sliding effect
    if (bEnergtic)
    {
        QObject::connect(q, &QAbstractButton::toggled, q, [q](bool checked)
        {
            if (checked)
            {
                q->setCheckState(Qt::Checked);
            }
            else
            {
                q->setCheckState(Qt::Unchecked);
            }
        });
    }

    QObject::connect(q, &USwitchButton::stateChanged, q, [this, q](int state)
    {
        if (state == Qt::Checked || state == Qt::PartiallyChecked)
        {
            if (m_lastState  == Qt::Unchecked)
            {
                QPropertyAnimation* animation = new QPropertyAnimation(q, "offset", q);
                animation->setStartValue(0);
                animation->setEndValue(q->width() - m_thumbWidth - q->margin() * 2);
                animation->setDuration(m_duration);
                animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
            }

            q->blockSignals(true);
            q->setChecked(true);
            q->blockSignals(false);
        }
        else
        {
            QPropertyAnimation* animation = new QPropertyAnimation(q, "offset", q);
            animation->setStartValue(m_offset);
            animation->setEndValue(0);
            animation->setDuration(m_duration);
            animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
            q->blockSignals(true);
            q->setChecked(false);
            q->blockSignals(false);
        }
    });
}

USwitchButton::USwitchButton(QWidget* parent, bool bEnergtic)
    : QAbstractButton(parent), d_ptr(new USwitchButtonPrivate(this))
{
    Q_D(USwitchButton);
    d_ptr->q_ptr = this;
    d->init(bEnergtic);
    setFixedSize(28, 16);
}

USwitchButton::~USwitchButton()
{
}

qreal USwitchButton::margin()
{
    Q_D(USwitchButton);
    return d->m_margin;
}

void USwitchButton::setMargin(qreal margin)
{
    Q_D(USwitchButton);
    d->m_margin = margin;
    update();
}

Qt::CheckState USwitchButton::checkState() const
{
    Q_D(const USwitchButton);
    return d->m_ckstate;
}

bool USwitchButton::isTristate() const
{
    Q_D(const USwitchButton);
    return d->m_isTristate;
}

void USwitchButton::setCheckState(Qt::CheckState state)
{
    Q_D(USwitchButton);

    if (d->m_ckstate != state)
    {
        d->m_lastState = d->m_ckstate;
        d->m_ckstate = state;
        emit stateChanged((int)state);
    }

    update();
}

void USwitchButton::setTristate(bool y)
{
    Q_D(USwitchButton);
    d->m_isTristate = y;
}

void USwitchButton::setCheckText(const QString &text)
{
    Q_D(USwitchButton);
    d->m_checkedText = text;
}

void USwitchButton::setNoCheckText(const QString &text)
{
    Q_D(USwitchButton);
    d->m_noCheckedText = text;
}

void USwitchButton::paintEvent(QPaintEvent*)
{
    Q_D(USwitchButton);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHints(QPainter::Antialiasing);

    isEnabled() ? painter.setOpacity(1) : painter.setOpacity(0.4);

    painter.save();

    if (d->m_ckstate == Qt::CheckState::Checked)
    {
        painter.setBrush(palette().brush(QPalette::Highlight));
        painter.drawRoundedRect(QRect(0, 0, width(), height()), height() / 2, height() / 2);
    }
    else if (d->m_ckstate == Qt::CheckState::Unchecked)
    {
        painter.setBrush(palette().brush(QPalette::Button));
        painter.drawRoundedRect(QRect(0, 0, width(), height()), height() / 2, height() / 2);
    }
    else
    {
        painter.setBrush(palette().brush(QPalette::Highlight));
        painter.drawRoundedRect(QRect(0, 0, width(), height()), height() / 2, height() / 2);
    }

    painter.restore();
    painter.setBrush(Qt::white);
    qreal radius = (height() - d->m_margin * 2) / 2;
    d->m_thumbWidth = radius * 2;
    painter.drawEllipse(QPointF(d->m_offset + d->m_margin + radius, height() / 2), radius, radius);
    if (d->m_ckstate == Qt::CheckState::PartiallyChecked)
    {
        painter.setPen(QPen(QColor(31, 129, 221), 2));
        painter.drawLine(QPointF(d->m_offset + d->m_margin + radius - d->m_lineWidth / 2, height() / 2),
                         QPointF(d->m_offset + d->m_margin + radius + d->m_lineWidth / 2, height() / 2));
    }
    // 将文字显示出来
#ifdef Q_OS_MAC
    QFont font = QFont("Times", 10);
# else
    QFont font = QFont("Arial", 10);
#endif
    QFontMetrics fm(font);
    painter.setFont(font);
    QPen pen;
    pen.setColor(palette().color(QPalette::WindowText));
    painter.setPen(pen);
    QRectF rect(this->rect());
    rect.setX(rect.x() + 4);
    rect.setWidth(rect.width() - 4);
    painter.drawText(rect, isChecked() ? (Qt::AlignLeft | Qt::AlignVCenter) : (Qt::AlignRight | Qt::AlignVCenter), isChecked() ? d->m_checkedText : d->m_noCheckedText);
}

void USwitchButton::enterEvent(QEvent*)
{
    Q_D(USwitchButton);
    d->m_hovering = true;
    setCursor(Qt::PointingHandCursor);
    update();
}

void USwitchButton::leaveEvent(QEvent*)
{
    Q_D(USwitchButton);
    d->m_hovering = false;
    setCursor(Qt::ArrowCursor);
    update();
}

void USwitchButton::checkStateSet()
{
    Q_D(USwitchButton);

    if (!signalsBlocked())
    {
        return;
    }

    if (d->m_lastState == Qt::PartiallyChecked || d->m_ckstate == Qt::PartiallyChecked)
    {
        return;
    }

    if (isChecked())
    {
        if (d->m_ckstate == Qt::Checked)
        {
            return;
        }

        d->m_ckstate = Qt::Checked;

        QPropertyAnimation* animation = new QPropertyAnimation(this, "offset", this);
        animation->setStartValue(0);
        animation->setEndValue(width() - height());
        animation->setDuration(d->m_duration);
        animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    }
    else
    {
        if (d->m_ckstate == Qt::Unchecked)
        {
            return;
        }

        d->m_ckstate = Qt::Unchecked;

        QPropertyAnimation* animation = new QPropertyAnimation(this, "offset", this);
        animation->setStartValue(width() - height());
        animation->setEndValue(0);
        animation->setDuration(d->m_duration);
        animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    }
}

int USwitchButton::offset()
{
    Q_D(USwitchButton);
    return d->m_offset;
}

void USwitchButton::setOffset(int offset)
{
    Q_D(USwitchButton);
    d->m_offset = offset;
    update();
}
