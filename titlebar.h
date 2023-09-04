#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QLabel;
class QPushButton;
class QMenu;
class StyleButton;
class QHBoxLayout;

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

    void setWindowTitle(const QString& title);
    void setTitleBarIcon(const QIcon& icon);

    void setMenuVisible(bool visible);
    void setMinButtonVisible(bool visible);
    void setMaxButtonVisible(bool visible);

    void setBackgroundColor(const QColor &color);

    void setMenu(QMenu* menu);
    QMenu* menu() const;

    void setRadius(int radius);

    void insertWidget(int index, QWidget *widget);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void updateMaximize();

signals:
    void windowStateChange();
    void minimizeButtonClicked();
    void maximizeButtonClicked();
    void closeButtonClicked();

private:
    QLabel *m_iconLabel = Q_NULLPTR;
    QLabel *m_titleLabel = Q_NULLPTR;
    StyleButton *m_meunBtn = Q_NULLPTR;
    StyleButton *m_minimizeButton = Q_NULLPTR;
    StyleButton *m_maximizeButton = Q_NULLPTR;
    StyleButton *m_closeButton = Q_NULLPTR;
    QHBoxLayout *m_layout = Q_NULLPTR;
    QPoint m_start;//起始点
    QPoint m_end;//结束点
    bool m_leftButtonPressed;//鼠标左键按下标记
    QSize m_size = QSize(15, 15);
    int m_radius = 15;
    QColor m_color = QColor();
    QMenu *m_menu = Q_NULLPTR;
};

#endif // TITLEBAR_H
