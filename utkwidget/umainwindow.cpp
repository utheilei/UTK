#include "umainwindow.h"
#include "titlebar.h"
#include "stylebutton.h"

#ifdef Q_OS_WIN
#include <windowsx.h>
#include <dwmapi.h>
#pragma comment (lib, "Dwmapi.lib")
#pragma comment (lib, "User32.lib")
#endif

#include <QStylePainter>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainterPath>
#include <QScreen>
#include <QWindow>
#include <QMenu>

UMainWindow::UMainWindow(QWidget* parent) : UWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMouseTracking(true);
    setMargins(8);
    addDropShadowEffect(true);
    
    m_titleBar = new UTitleBar(this);
    installEventFilter(m_titleBar);
    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setContentsMargins(8, 8, 8, 8);
    m_mainLayout->addWidget(m_titleBar);
    m_mainLayout->addLayout(m_layout);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    setWindowTitle(qApp->applicationName());
    setWindowIcon(qApp->windowIcon());
    initConnections();
    initWindowLong();
}

void UMainWindow::initConnections()
{
    connect(m_titleBar, &UTitleBar::windowStateChange, this, &UMainWindow::onWindowStateChange);
    connect(m_titleBar, &UTitleBar::minimizeButtonClicked, this, &UMainWindow::onMinimizeButtonClicked);
    connect(m_titleBar, &UTitleBar::maximizeButtonClicked, this, &UMainWindow::onMaximizeButtonClicked);
    connect(m_titleBar, &UTitleBar::closeButtonClicked, this, &UMainWindow::onCloseButtonClicked);
}

void UMainWindow::addMenu(QMenu* menu)
{
    m_titleBar->setMenu(menu);
}

void UMainWindow::insertWidget(int index, QWidget* widget)
{
    m_mainLayout->insertWidget(index, widget);
}

void UMainWindow::initWindowLong()
{
#ifdef Q_OS_WIN
    HWND hwnd = HWND(winId());
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_SYSMENU;
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    auto window = this->windowHandle();
    connect(window, &QWindow::screenChanged, this, [ = ]
    {
        auto hWnd = reinterpret_cast<HWND>(window->winId());
        SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    });
#endif
}

void UMainWindow::onWindowStateChange()
{
    StyleButton* closeButton = m_titleBar->findChild<StyleButton*>("closeButton");
    if (this->isMaximized())
    {
        setMargins(0);
        setRadius(0);
        addDropShadowEffect(false);
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_titleBar->setRadius(0);
        if (closeButton)
            closeButton->setRadius(0);
    }
    else
    {
        setMargins(8);
        setRadius(15);
        addDropShadowEffect(true);
        m_titleBar->setRadius(15);
        m_mainLayout->setContentsMargins(8, 8, 8, 8);
        if (closeButton)
            closeButton->setRadius(15);
    }
}

void UMainWindow::onMinimizeButtonClicked()
{
    showMinimized();
}

void UMainWindow::onMaximizeButtonClicked()
{
    isMaximized() ? showNormal() : showMaximized();
}

void UMainWindow::onCloseButtonClicked()
{
    close();
}

UTitleBar* UMainWindow::titleBar() const
{
    return m_titleBar;
}

void UMainWindow::setContentWidget(QWidget* widget)
{
    if (m_contentWidget == Q_NULLPTR)
    {
        m_layout->addWidget(widget);
        m_contentWidget = widget;
    }
    else
    {
        m_layout->removeWidget(m_contentWidget);
        m_contentWidget->deleteLater();
        m_layout->addWidget(widget);
        m_contentWidget = widget;
    }
}

QWidget* UMainWindow::contentWidget() const
{
    return m_contentWidget;
}

void UMainWindow::moveToCenter()
{
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

#ifndef Q_OS_WIN
int UMainWindow::setCursorStyle(const QPoint &curPoint)
{
    int nCurWidth = this->width();
    int nCurHeight = this->height();
    int nRes = CursorNormal;

    if ((nCurWidth - curPoint.x() <= 3) && (nCurHeight - curPoint.y() <= 3))
    {
        setCursor(Qt::SizeFDiagCursor);
        nRes = CursorBottomRight;
    }
    else if (curPoint.x() <= 3 && curPoint.y() <= 3)
    {
        setCursor(Qt::SizeFDiagCursor);
        nRes = CursorTopLeft;
    }
    else if (curPoint.x() <= 3 && (nCurHeight - curPoint.y() <= 3))
    {
        setCursor(Qt::SizeBDiagCursor);
        nRes = CursorBottomLeft;
    }
    else if ((nCurWidth - curPoint.x() <= 3) && curPoint.y() <= 3)
    {
        setCursor(Qt::SizeBDiagCursor);
        nRes = CursorTopRight;
    }
    else if (nCurWidth - curPoint.x() <= 3)
    {
        setCursor(Qt::SizeHorCursor);
        nRes = CursorRight;
    }
    else if (nCurHeight - curPoint.y() <= 3)
    {
        setCursor(Qt::SizeVerCursor);
        nRes = CursorBottom;
    }
    else if (curPoint.x() <= 3)
    {
        setCursor(Qt::SizeHorCursor);
        nRes = CursorLeft;
    }
    else if (curPoint.y() <= 3)
    {
        setCursor(Qt::SizeVerCursor);
        nRes = CursorTop;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        nRes = CursorNormal;
    }

    return nRes;
}
#endif

void UMainWindow::mousePressEvent(QMouseEvent* event)
{
#ifndef Q_OS_WIN
    if (event->button() == Qt::LeftButton)
    {
        m_MousePressPos = event->globalPos();
        m_bMousePressed = true;
    }

    if (CursorNormal != m_nMouseResize)
    {
        m_bMouseResizePressed = true;
    }
#else
    UWidget::mousePressEvent(event);
#endif
}

void UMainWindow::mouseReleaseEvent(QMouseEvent* event)
{
#ifndef Q_OS_WIN
    if (event->button() == Qt::LeftButton)
    {
        m_bMousePressed = false;
        m_bMouseResizePressed = false;
        m_nMouseResize = setCursorStyle(event->pos());
    }
#else
    UWidget::mouseReleaseEvent(event);
#endif
}

void UMainWindow::mouseMoveEvent(QMouseEvent* event)
{
#ifndef Q_OS_WIN
    if (Qt::WindowMaximized == windowState())
        return;

    QPoint curPoint = event->pos();
    if (!m_bMouseResizePressed)
        m_nMouseResize = setCursorStyle(curPoint);

    if (m_bMousePressed && (event->buttons() == Qt::LeftButton))
    {
        m_MouseMovePos = event->globalPos();
        QPoint movePoint = m_MouseMovePos - m_MousePressPos;
        if (CursorNormal != m_nMouseResize)
        {
            QRect rect = geometry();
            switch (m_nMouseResize)
            {
                case CursorTopLeft:
                    rect.setTopLeft(rect.topLeft() + movePoint);
                    break;
                case CursorTop:
                    rect.setTop(rect.top() + movePoint.y());
                    break;
                case CursorTopRight:
                    rect.setTopRight(rect.topRight() + movePoint);
                    break;
                case CursorLeft:
                    rect.setLeft(rect.left() + movePoint.x());
                    break;
                case CursorBottomLeft:
                    rect.setBottomLeft(rect.bottomLeft() + movePoint);
                    break;
                case CursorBottom:
                    rect.setBottom(rect.bottom() + movePoint.y());
                    break;
                case CursorBottomRight:
                    rect.setBottomRight(rect.bottomRight() + movePoint);
                    break;
                case CursorRight:
                    rect.setRight(rect.right() + movePoint.x());
                    break;
                default:
                    break;
            }

            if (rect.width() <= minimumWidth() && rect.height() <= minimumHeight())
                return;

            setGeometry(rect);
        }
        m_MousePressPos = event->globalPos();
    }
#else
    UWidget::mouseMoveEvent(event);
#endif
}

#ifdef Q_OS_WIN
bool UMainWindow::nativeEvent(const QByteArray &eventType, void* message, long* result)
{
    MSG* msg = (MSG*)message;

    switch (msg->message)
    {
        case WM_NCCALCSIZE: // 移除标题栏
        {
            *result = WVR_REDRAW;
            return true;
        }
        case WM_NCHITTEST:  // 命中检测，增加窗口缩放
        {
            const LONG borderWidth = margins();
            const LONG cornerWidth = margins() + 8;
            RECT winrect;
            GetWindowRect(HWND(effectiveWinId()), &winrect);
            long x = GET_X_LPARAM(msg->lParam);
            long y = GET_Y_LPARAM(msg->lParam);

            bool resizeWidth = minimumWidth() != maximumWidth();
            bool resizeHeight = minimumHeight() != maximumHeight();

            if (resizeWidth)
            {
                //left border
                if (x >= winrect.left && x < winrect.left + borderWidth)
                {
                    *result = HTLEFT;
                }
                //right border
                if (x < winrect.right && x >= winrect.right - borderWidth)
                {
                    *result = HTRIGHT;
                }
            }
            if (resizeHeight)
            {
                //bottom border
                if (y < winrect.bottom && y >= winrect.bottom - borderWidth)
                {
                    *result = HTBOTTOM;
                }
                //top border
                else if (y >= winrect.top && y < winrect.top + borderWidth)
                {
                    *result = HTTOP;
                }
            }
            if (resizeWidth && resizeHeight)
            {
                //bottom left corner
                if (x >= winrect.left && x < winrect.left + cornerWidth &&
                    y < winrect.bottom && y >= winrect.bottom - cornerWidth)
                {
                    *result = HTBOTTOMLEFT;
                }
                //bottom right corner
                else if (x < winrect.right && x >= winrect.right - cornerWidth &&
                         y < winrect.bottom && y >= winrect.bottom - cornerWidth)
                {
                    *result = HTBOTTOMRIGHT;
                }
                //top left corner
                else if (x >= winrect.left && x < winrect.left + cornerWidth &&
                         y >= winrect.top && y < winrect.top + cornerWidth)
                {
                    *result = HTTOPLEFT;
                }
                //top right corner
                else if (x < winrect.right && x >= winrect.right - cornerWidth &&
                         y >= winrect.top && y < winrect.top + cornerWidth)
                {
                    *result = HTTOPRIGHT;
                }
            }
            if (0 != *result)
            {
                return true;
            }
            if (!m_titleBar)
            {
                return false;
            }

            // support highdpi
            double dpr = this->devicePixelRatioF();
            QPoint pos = m_titleBar->mapFromGlobal(QPoint(x / dpr, y / dpr));

            if (!m_titleBar->rect().contains(pos))
            {
                return false;
            }
            QWidget* child = m_titleBar->childAt(pos);
            if (!child)
            {
                *result = HTCAPTION;
                return true;
            }
            return false;
        }
        case WM_SIZE:   // 窗口大小判断
        {
            if (!m_titleBar)
            {
                return false;
            }

            double dpr = this->devicePixelRatioF();

            WINDOWPLACEMENT wp;
            GetWindowPlacement(HWND(effectiveWinId()), &wp);
            if (wp.showCmd == SW_MAXIMIZE)
            {
                UWidget::setContentsMargins(8 / dpr, 8 / dpr, 8 / dpr, 8 / dpr);
            }
            else
            {
                UWidget::setContentsMargins(0, 0, 0, 0);
            }
            return false;
        }
        default:
            break;
    }

    return false;
}
#endif
