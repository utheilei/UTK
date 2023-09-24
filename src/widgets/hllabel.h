#pragma once

#include "utkwidget_export.h"

#include <QLabel>

class UTKWIDGET_EXPORT ToolTip : public QLabel
{
    Q_OBJECT
public:
    explicit ToolTip(QWidget* parent = nullptr);
    ~ToolTip();

    void setRadius(int radius);

    void setToolTipFont(const QFont &font);

    void setBackgroundColor(const QBrush &brush);

    void setTextColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent* event);

private:
    int m_radius = 8;
};
