#include "titlebar.h"
#include "stylebutton.h"

#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QStyle>
#include <QMenu>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(50);
    setWindowFlags(Qt::FramelessWindowHint);
    setObjectName("titleBar");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(35, 35);
    m_iconLabel->setScaledContents(true);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFont font("SimHei", 15);
    m_titleLabel->setFont(font);

    m_meunBtn = new StyleButton("", this);
    m_meunBtn->setFixedSize(QSize(height(), height()));
    m_meunBtn->setObjectName("menuButton");
    m_meunBtn->setIcon(QIcon::fromTheme("menu"));
    m_meunBtn->setIconSize(m_size);
    m_meunBtn->setFlat(true);

    m_minimizeButton = new StyleButton("", this);
    m_minimizeButton->setFixedSize(QSize(height(), height()));
    m_minimizeButton->setObjectName("minimizeButton");
    m_minimizeButton->setIcon(QIcon::fromTheme("min"));
    m_minimizeButton->setIconSize(m_size);
    m_minimizeButton->setFlat(true);

    m_maximizeButton = new StyleButton("",this);
    m_maximizeButton->setFixedSize(QSize(height(), height()));
    m_maximizeButton->setObjectName("maximizeButton");
    m_maximizeButton->setIcon(QIcon::fromTheme("max"));
    m_maximizeButton->setIconSize(m_size);
    m_maximizeButton->setFlat(true);

    m_closeButton = new StyleButton("", this);
    m_closeButton->setFixedSize(QSize(height(), height()));
    m_closeButton->setObjectName("closeButton");
    m_closeButton->setIcon(QIcon::fromTheme("close"));
    m_closeButton->setIconSize(m_size);
    m_closeButton->setFlat(true);
    m_closeButton->setRadius(15);
    m_closeButton->setRadiuPosition(StyleButton::TopRight);

    m_minimizeButton->setToolTip("Minimize");
    m_maximizeButton->setToolTip("Maximize");
    m_closeButton->setToolTip("Close");

    m_layout = new QHBoxLayout;
    m_layout->addSpacing(10);
    m_layout->addWidget(m_iconLabel);
    m_layout->addStretch();
    m_layout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    m_layout->addStretch();
    m_layout->addWidget(m_meunBtn);
    m_layout->addWidget(m_minimizeButton);
    m_layout->addWidget(m_maximizeButton);
    m_layout->addWidget(m_closeButton);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);

    connect(m_minimizeButton, &QPushButton::clicked, this, &TitleBar::minimizeButtonClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &TitleBar::maximizeButtonClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &TitleBar::closeButtonClicked);
    connect(m_meunBtn, &QPushButton::clicked, this, [=](){
        QPoint pos = mapToGlobal(m_meunBtn->geometry().bottomLeft());
        if (m_menu)
            m_menu->exec(pos);
    });
}

void TitleBar::setWindowTitle(const QString &title)
{
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setText(title);
}

void TitleBar::setTitleBarIcon(const QIcon& icon)
{
    m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
}

void TitleBar::setMenuVisible(bool visible)
{
    m_meunBtn->setVisible(visible);
}

void TitleBar::setMinButtonVisible(bool visible)
{
    m_minimizeButton->setVisible(visible);
}

void TitleBar::setMaxButtonVisible(bool visible)
{
    m_maximizeButton->setVisible(visible);
}

void TitleBar::setBackgroundColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        update();
    }
}

void TitleBar::setMenu(QMenu *menu)
{
    if (m_menu) {
        m_menu->deleteLater();
        m_menu = Q_NULLPTR;
    }

    m_menu = menu;
}

QMenu *TitleBar::menu() const
{
    return m_menu;
}

void TitleBar::setRadius(int radius)
{
    if (m_radius != radius) {
        m_radius = radius;
        update();
    }
}

void TitleBar::insertWidget(int index, QWidget *widget)
{
    m_layout->insertWidget(index, widget);
}

void TitleBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    QPainterPath rectPath;
    rectPath.setFillRule(Qt::WindingFill);
    if (m_radius > 0) {
        rectPath.addRoundedRect(rect(), m_radius, m_radius);
        rectPath.addRect(QRect(0, height()/2, width(), height()));
    } else {
        rectPath.addRect(rect());
    }
    painter.setClipPath(rectPath);

    painter.fillPath(rectPath, m_color.isValid() ? m_color : palette().color(QPalette::Base));
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_maximizeButton->isVisible())
        m_maximizeButton->click();

    QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    // 鼠标左键按下事件
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标左键状态
        m_leftButtonPressed = true;
        //记录鼠标在屏幕中的位置
        m_start = event->globalPos();
    }

    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    // 持续按住才做对应事件
    if(m_leftButtonPressed) {
        //将父窗体移动到父窗体原来的位置加上鼠标移动的位置：event->globalPos()-m_start
        if (parentWidget())
            parentWidget()->move(parentWidget()->geometry().topLeft() +
                                 event->globalPos() - m_start);
        //将鼠标在屏幕中的位置替换为新的位置
        m_start = event->globalPos();
    }

    QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标左键释放
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标状态
        m_leftButtonPressed = false;
    }

    QWidget::mouseReleaseEvent(event);
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type()) {
    //设置标题
    case QEvent::WindowTitleChange: {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_titleLabel->setText(pWidget->windowTitle());
            return true;
        }
        break;
    }
        //设置图标
    case QEvent::WindowIconChange: {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget) {
            QIcon icon = pWidget->windowIcon();
            m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
            return true;
        }
        break;
    }
        // 窗口状态变化、窗口大小变化
    case QEvent::WindowStateChange: {
        emit windowStateChange();
        break;
    }
    case QEvent::Resize: {
        updateMaximize();
        return true;
    }
    }

    return QWidget::eventFilter(obj, event);
}

void TitleBar::resizeEvent(QResizeEvent *event)
{
    m_titleLabel->move((this->width()-m_titleLabel->width())/2,
                       (this->height()-m_titleLabel->height())/2);

    QWidget::resizeEvent(event);
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel()) {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize) {
            m_maximizeButton->setToolTip(tr("Restore"));
            m_maximizeButton->setProperty("maximizeProperty", "restore");
            m_maximizeButton->setIcon(QIcon::fromTheme("max1"));
        } else {
            m_maximizeButton->setProperty("maximizeProperty", "maximize");
            m_maximizeButton->setToolTip(tr("Maximize"));
            m_maximizeButton->setIcon(QIcon::fromTheme("max"));
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}
