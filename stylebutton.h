#ifndef STYLEBUTTON_H
#define STYLEBUTTON_H

#include <QPushButton>
#include <QEvent>

class StyleButton : public QPushButton
{
public:
    enum RadiuPosition {
        Normal = 0,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    StyleButton(const QString text, QWidget *parent = Q_NULLPTR);

    void setRadius(int radius);

    void setHoverColor(QColor color);

    void setRadiuPosition(RadiuPosition position);

protected:
    void enterEvent(QEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_isEnter = false;
    int m_radius = 0;
    QColor m_color = QColor();
    RadiuPosition m_position = Normal;
};

#endif // STYLEBUTTON_H
