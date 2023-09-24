#include "udialog.h"
#include "titlebar.h"
#include "uapplication.h"
#include "style/upalette.h"

#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

UDialog::UDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumWidth(420);

    titleBar = new TitleBar(this);
    titleBar->setMenuVisible(false);
    titleBar->setMinButtonVisible(false);
    titleBar->setMaxButtonVisible(false);
    setContentsMargins(margin, margin, margin, margin);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(titleBar, 0, Qt::AlignTop);
    setLayout(m_mainLayout);

    connect(titleBar, &TitleBar::closeButtonClicked, this, &UDialog::reject);
}

void UDialog::setTitleIcon(const QIcon &icon)
{
    titleBar->setTitleBarIcon(icon);
}

void UDialog::setMessageBoxTitle(const QString &title)
{
    titleBar->setWindowTitle(title);
}

void UDialog::setContentWidget(QWidget* widget)
{
    m_mainLayout->addWidget(widget);
}

void UDialog::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    QRect contentRect(rect().topLeft().x() + margin, rect().topLeft().y() + margin,
                      (rect().width() - 2 * margin), (rect().height() - 2 * margin));
    rectPath.addRoundedRect(contentRect, 15, 15);
    painter.setPen(QPen(palette().color(QPalette::Base), 1));
    painter.setBrush(palette().color(QPalette::Base));
    painter.drawPath(rectPath);

    QColor color = uApp->applicationPalette()->color(QPalette::Shadow);
    int arr[8] = {10, 20, 30, 40, 50, 80, 120, 150};
    for (int i = 0; i < margin; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(rect().topLeft().x() + i, rect().topLeft().y() + i,
                            (rect().width() - 2 * i), (rect().height() - 2 * i), 15, 15);

        color.setAlpha(arr[i]);
        painter.setPen(color);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }

    QDialog::paintEvent(event);
}
