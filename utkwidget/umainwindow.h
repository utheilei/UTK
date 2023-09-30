#ifndef UMAINWINDOW_H
#define UMAINWINDOW_H

#include "uwidget.h"

class UTitleBar;
class QVBoxLayout;
class QMenu;

class UTKWIDGET_EXPORT UMainWindow : public UWidget
{
    Q_OBJECT
public:
    explicit UMainWindow(QWidget* parent = nullptr);

#ifndef Q_OS_WIN
    enum tagCursorCtrlStyle
    {
        CursorNormal = 0,    // 普通鼠标
        CursorTopLeft,
        CursorTop,
        CursorTopRight,
        CursorLeft,
        CursorBottomLeft,
        CursorBottom,
        CursorBottomRight,
        CursorRight
    };

    int setCursorStyle(const QPoint &curPoint);    // 当前位置设置鼠标样式
#endif

    void setContentWidget(QWidget* widget);

    QWidget* contentWidget() const;
    
    UTitleBar* titleBar() const;

    void moveToCenter();

    void addMenu(QMenu* menu);

    void insertWidget(int index, QWidget* widget);

private:
    void initConnections();
    void initWindowLong();

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray &eventType, void* message, long* result) Q_DECL_OVERRIDE;
#endif

signals:

public slots:
    void onWindowStateChange();
    void onMinimizeButtonClicked();
    void onMaximizeButtonClicked();
    void onCloseButtonClicked();

private:
    QPoint m_MousePressPos;                 // 鼠标点击位置
    QPoint m_MouseMovePos;                  // 鼠标移动位置
    bool m_bMousePressed = false;         // 鼠标是否按下
    int m_nMouseResize;                  // 鼠标设置大小
    bool m_bMouseResizePressed = false;   // 设置大小的按下
    UTitleBar* m_titleBar = Q_NULLPTR;
    QVBoxLayout* m_layout = Q_NULLPTR;
    QVBoxLayout* m_mainLayout = Q_NULLPTR;
    QWidget* m_contentWidget = Q_NULLPTR;
};

#endif // UMAINWINDOW_H
