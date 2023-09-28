#ifndef HLPASSWORDLINEEDIT_H
#define HLPASSWORDLINEEDIT_H

#include "utkwidget_export.h"

#include <QWidget>

class HLLineEdit;
class LineEditButton;
class UTKWIDGET_EXPORT HLPassWordLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit HLPassWordLineEdit(QWidget* parent = nullptr);

    HLLineEdit* lineEidt() const;

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:

public slots:

private:
    HLLineEdit* lineEdit = nullptr;
    LineEditButton* button = nullptr;
};

#endif // HLPASSWORDLINEEDIT_H
