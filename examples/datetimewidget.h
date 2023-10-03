#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include "uwidget.h"

class DateTimeWidget : public UWidget
{
    Q_OBJECT
public:
    DateTimeWidget(QWidget *parent = nullptr);
    ~DateTimeWidget();
};

#endif // DATETIMEWIDGET_H
