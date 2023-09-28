#ifndef HLSLIDER_H
#define HLSLIDER_H

#include "utkwidget_export.h"

#include <QSlider>

class UProxyStyle;
class UTKWIDGET_EXPORT HLSlider : public QSlider
{
    Q_OBJECT
public:
    explicit HLSlider(QWidget* parent = nullptr);
    explicit HLSlider(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* ev) override;

private:
    QScopedPointer<UProxyStyle> m_style;
};

#endif // HLSLIDER_H
