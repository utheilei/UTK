#ifndef UWIDGET_H
#define UWIDGET_H

#include "utkwidget_export.h"

#include <QWidget>

class UTKWIDGET_EXPORT UWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UWidget(QWidget* parent = nullptr);

    void addDropShadowEffect(bool isOpen);

    int margins() const;

    void setMargins(int margins);

    void setRadius(int radius);

    void setBackgroundColor(const QBrush &brush);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    bool m_isOpen = false;
    int m_margins = 0;
    int m_radius = 15;
};

#endif // UWIDGET_H
