#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include "uwidget.h"

class ViewWidget : public UWidget
{
    Q_OBJECT
public:
    explicit ViewWidget(QWidget* parent = nullptr);
    ~ViewWidget();
};

#endif // VIEWWIDGET_H
