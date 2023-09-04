#ifndef HLSLIDER_H
#define HLSLIDER_H

#include <QSlider>

class HLSlider : public QSlider
{
    Q_OBJECT
public:
    explicit HLSlider(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif // HLSLIDER_H
