//鼠标hover灰色背景，以及点击后高亮
#ifndef HLSTYLEBUTTON_H
#define HLSTYLEBUTTON_H

#include "utkwidget_export.h"

#include <QPushButton>
#include <QEvent>

class UTKWIDGET_EXPORT HLStyleButton : public QPushButton
{
public:
    HLStyleButton(const QString text, QWidget* parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};

#endif // HLSTYLEBUTTON_H
