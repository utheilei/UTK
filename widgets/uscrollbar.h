#ifndef USCROLLBAR_H
#define USCROLLBAR_H

#include <QScrollBar>
#include <QPropertyAnimation>

class UScrollBar : public QScrollBar
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    explicit UScrollBar(QWidget* parent = nullptr);
    explicit UScrollBar(Qt::Orientation, QWidget* parent = nullptr);

    qreal opacity() const;

    void setOpacity(qreal opacity);

signals:
    void opacityChanged();

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void initAnimation();
    void handleAnimation(qreal startValue, qreal endValue);
    QPropertyAnimation m_propertyAnimation;
    qreal m_opacity = 0.0;
};

#endif // USCROLLBAR_H
