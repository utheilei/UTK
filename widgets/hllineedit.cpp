#include "hllineedit.h"
#include "hllabel.h"
#include "widgetutils.h"

#include <QPainter>
#include <QStyleOptionFrame>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QAction>
#include <QTimer>
#include <QToolButton>
#include <QStylePainter>
#include <QPainterPath>

LineEditButton::LineEditButton(QWidget* parent)
    : QPushButton(parent)
{
    setFlat(true);
    setCheckable(true);
}

void LineEditButton::setRadius(int radius)
{
    if (m_radius != radius)
    {
        m_radius = radius;
        update();
    }
}

void LineEditButton::setBackGroundColor(const QColor &color)
{
    if (m_color != color)
    {
        m_color = color;
        update();
    }
}

void LineEditButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QStyleOptionButton btn;
    initStyleOption(&btn);

    QStylePainter stylePainter(this);
    stylePainter.setRenderHint(QPainter::Antialiasing);

    QColor brush = m_color.isValid() ? m_color : palette().color(QPalette::Button);
    stylePainter.setBrush(brush);
    stylePainter.setPen(Qt::NoPen);
    if (btn.state & QStyle::State_Enabled)
    {
        if (btn.state & QStyle::State_MouseOver)
            stylePainter.setBrush(brush.darker(120));
    }

    if (isDown())
        stylePainter.setBrush(brush.darker(150));

    QPainterPath painterPath;
    painterPath.addRoundedRect(rect(), m_radius, m_radius);
    stylePainter.drawPath(painterPath);

    style()->drawControl(QStyle::CE_PushButtonLabel, &btn, &stylePainter, this);
}

HLLineEdit::HLLineEdit(QWidget* parent) : QLineEdit(parent)
{
    initLineEdit();
    initToolTip();
    initConnection();
}

HLLineEdit::HLLineEdit(const QString &text, QWidget* parent) : QLineEdit(text, parent)
{
    initLineEdit();
    initToolTip();
    initConnection();
}

HLLineEdit::~HLLineEdit()
{
}

void HLLineEdit::initLineEdit()
{
    setFrame(false);
    setMinimumHeight(35);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    auto pa = palette();
    pa.setBrush(QPalette::Base, Qt::transparent);
    setPalette(pa);

    QHBoxLayout* hLayout = new QHBoxLayout;
    setLayout(hLayout);

    label = new QLabel(this);
    label->setVisible(false);
    label->setObjectName("HLLineEdit_LeftAction");
    hLayout->addWidget(label);
    hLayout->setAlignment(label, Qt::AlignLeft | Qt::AlignVCenter);

    closeButton = new LineEditButton(this);
    closeButton->setVisible(false);
    closeButton->setBackGroundColor(Qt::white);
    closeButton->setRadius(30);
    closeButton->setIcon(QIcon(QPixmap(":/images/close.svg").scaled(10, 10, Qt::KeepAspectRatio)));
    closeButton->setObjectName("HLLineEdit_CloseButton");
    hLayout->addWidget(closeButton);
    hLayout->setAlignment(closeButton, Qt::AlignRight | Qt::AlignVCenter);
}

void HLLineEdit::initToolTip()
{
    toolTip = new ToolTip(this);
    toolTip->setVisible(false);
    toolTip->setWindowFlags(windowFlags() | Qt::ToolTip | Qt::FramelessWindowHint);
    toolTip->setAttribute(Qt::WA_TranslucentBackground);
    toolTip->setTextColor(QColor(241, 57, 50));
    //    toolTip->setBackgroundColor(palette().color(QPalette::Base));
}

void HLLineEdit::initConnection()
{
    connect(this, &HLLineEdit::textChanged, this, [ = ](const QString &)
    {
        closeButton->setVisible(!text().isEmpty());
    });

    connect(closeButton, &LineEditButton::clicked, this, [ = ]()
    {
        this->clear();
        closeButton->setVisible(false);
        this->setFocus();
    });
}

void HLLineEdit::setRadius(int radius)
{
    if (m_radius != radius)
    {
        m_radius = radius;
        update();
    }
}

bool HLLineEdit::closeButtonVisible()
{
    return closeButton->isVisible();
}

void HLLineEdit::addLeftAction(const QPixmap &pixmap)
{
    if (!pixmap.isNull())
    {
        label->setPixmap(pixmap);
        label->setVisible(true);
    }
}

void HLLineEdit::addLeftAction(const QString &text)
{
    if (!text.trimmed().isEmpty())
    {
        label->setText(text);
        label->setVisible(true);
    }
}

void HLLineEdit::clearAction()
{
    label->setVisible(false);
}

bool HLLineEdit::isAlert()
{
    return m_isAlert;
}

void HLLineEdit::showAlertMessage(const QString &text, int duration)
{
    if ((toolTip && toolTip->isVisible()) || text.trimmed().isEmpty())
        return;

    toolTip->move(mapToGlobal(rect().bottomLeft() + QPoint(0, 2)));
    toolTip->setText(text);
    toolTip->setVisible(true);
    m_isAlert = true;
    update();

    QTimer::singleShot(duration, this, &HLLineEdit::hideAlertMessage);
}

void HLLineEdit::hideAlertMessage()
{
    m_isAlert = false;
    if (toolTip)
        toolTip->setVisible(false);

    update();
}

void HLLineEdit::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    QRect rect = this->rect().adjusted(1.5, 1.5, -1.5, -1.5);
    if (m_radius > 0)
        rectPath.addRoundedRect(rect, m_radius, m_radius);
    else
        rectPath.addRect(rect);

    QColor backGround = m_isAlert ? QColor(241, 57, 50, 0.15 * 255) : palette().color(QPalette::Button);
    painter.fillPath(rectPath, backGround);

    QLineEdit::paintEvent(event);

    if (hasFocus())
    {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(palette().color(QPalette::Highlight), 1.5));
        painter.drawPath(rectPath);
        painter.restore();
    }
}

void HLLineEdit::resizeEvent(QResizeEvent* event)
{
    int width = height() - layout()->contentsMargins().top() - layout()->contentsMargins().bottom();
    closeButton->setFixedSize(QSize(width, width));
    auto margins = textMargins();
    int leftPadding = 6;
    int rightPadding = 6;
    if (label->isVisible())
        leftPadding = leftPadding + label->width() + 5;
    if (closeButtonVisible())
        rightPadding = rightPadding + closeButton->width() + 5;

    setTextMargins(leftPadding, margins.top(), rightPadding, margins.bottom());

    QLineEdit::resizeEvent(event);
}
