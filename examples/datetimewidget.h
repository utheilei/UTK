#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include <QWidget>

class DateTimeWidget : public QWidget
{
    Q_OBJECT
public:
    DateTimeWidget(QWidget *parent = nullptr);
    ~DateTimeWidget();
};

#endif // DATETIMEWIDGET_H
