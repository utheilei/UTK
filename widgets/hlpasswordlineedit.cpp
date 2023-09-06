#include "hlpasswordlineedit.h"
#include "hllineedit.h"

#include <QHBoxLayout>

HLPassWordLineEdit::HLPassWordLineEdit(QWidget* parent) : QWidget(parent)
{
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    setLayout(hlayout);

    lineEdit = new HLLineEdit(this);
    button = new LineEditButton(this);
    button->setBackGroundColor(palette().color(QPalette::Highlight));
    button->setIcon(QIcon(":/images/passwordshow.svg"));

    hlayout->addWidget(lineEdit);
    hlayout->addSpacing(5);
    hlayout->addWidget(button);

    connect(button, &LineEditButton::clicked, this, [ = ](bool checked)
    {
        QLineEdit::EchoMode mode = checked ? QLineEdit::EchoMode::Password : QLineEdit::EchoMode::Normal;
        QIcon icon = checked ? QIcon(":/images/passwordshow.svg") : QIcon(":/images/passwordhide.svg");
        button->setIcon(icon);
        lineEdit->setEchoMode(mode);
        lineEdit->setFocus();
    });
}

HLLineEdit* HLPassWordLineEdit::lineEidt() const
{
    return lineEdit;
}

void HLPassWordLineEdit::resizeEvent(QResizeEvent* event)
{
    button->setFixedSize(QSize(lineEdit->height(), lineEdit->height()));
    QWidget::resizeEvent(event);
}
