#include "ucheckbox.h"
#include "style/ustylepainter.h"

#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionButton>

class UCheckBoxPrivate
{
public:
    UCheckBoxPrivate(UCheckBox* q) : q_ptr(q) {}
    ~UCheckBoxPrivate() {};

private:
    Q_DECLARE_PUBLIC(UCheckBox)
    Q_DISABLE_COPY(UCheckBoxPrivate)
    UCheckBox* const q_ptr = nullptr;
};

UCheckBox::UCheckBox(QWidget* parent) : QCheckBox(parent)
    , d_ptr(new UCheckBoxPrivate(this))
{
    connect(this, &UCheckBox::toggled, this, [ = ](bool checked)
    {
        Qt::CheckState state = checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
        setCheckState(state);
    });
}

UCheckBox::UCheckBox(const QString &text, QWidget* parent) : QCheckBox(text, parent)
    , d_ptr(new UCheckBoxPrivate(this))
{
    connect(this, &UCheckBox::toggled, this, [ = ](bool checked)
    {
        Qt::CheckState state = checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
        setCheckState(state);
    });
}

UCheckBox::~UCheckBox()
{
}

void UCheckBox::paintEvent(QPaintEvent* event)
{
    UStylePainter painter(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    painter.drawControl(UProxyStyle::UControlElement::CE_CheckBox, opt);
}
