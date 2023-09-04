#include "tooltipsbutton.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMovie>

ToolTipsButton::ToolTipsButton(QWidget *parent, Qt::Orientation orientation) : QWidget(parent)
{
    if (Qt::Vertical == orientation)
        m_mainLayout = new QVBoxLayout;
    else if (Qt::Horizontal == orientation) {
        m_mainLayout = new QHBoxLayout;
    }

    m_iconLabel = new QLabel(this);
    m_textLabel = new QLabel(this);

    m_mainLayout->addWidget(m_iconLabel);
    m_mainLayout->setAlignment(m_iconLabel, Qt::AlignCenter);
    m_mainLayout->addWidget(m_textLabel);
    m_mainLayout->setAlignment(m_textLabel, Qt::AlignCenter);
    setLayout(m_mainLayout);

    connect(&timer, &QTimer::timeout, this, [this](){
        QString path = QString(":/images/loading/monitorloading_%1.png").arg(count);
        setIcon(QIcon(path));
        count++;
        if (26 == count)
            count = 0;
    });
}

void ToolTipsButton::setText(const QString &text)
{
    m_textLabel->setText(text);
}

void ToolTipsButton::setIconSize(const QSize &size)
{
    if (m_size != size) {
        m_size = size;
        m_iconLabel->setFixedSize(size);
    }
}

void ToolTipsButton::setIcon(const QIcon &icon)
{
    m_iconLabel->setPixmap(icon.pixmap(m_size));
}

void ToolTipsButton::setSpacing(int space)
{
    m_mainLayout->setSpacing(space);
}

void ToolTipsButton::setMovie(const QString &filePath)
{
    m_iconLabel->clear();
    m_pMovie = new QMovie(filePath);
    m_pMovie->setParent(m_iconLabel);
    m_iconLabel->setMovie(m_pMovie);
    m_pMovie->start();
}

void ToolTipsButton::startMovie()
{
    if (QMovie::Running != m_pMovie->state())
        m_pMovie->start();
}

void ToolTipsButton::stopMovie()
{
    if (QMovie::Running == m_pMovie->state())
        m_pMovie->stop();
}

void ToolTipsButton::startGif()
{
    if (!timer.isActive())
        timer.start(40);
}

void ToolTipsButton::stopGif()
{
    if (timer.isActive()) {
        timer.stop();
        count = 0;
    }
}

void ToolTipsButton::setIconLabelVisible(bool isVisible)
{
    m_iconLabel->setVisible(isVisible);
}

void ToolTipsButton::setTextLabelVisible(bool isVisible)
{
    m_textLabel->setVisible(isVisible);
}
