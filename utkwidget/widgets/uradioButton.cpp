#include "uradioButton.h"
#include "style/ustylepainter.h"

#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionButton>

class URadioButtonPrivate
{
public:
    URadioButtonPrivate(URadioButton* q) : q_ptr(q) {}
    ~URadioButtonPrivate() {};

private:
    Q_DECLARE_PUBLIC(URadioButton)
    Q_DISABLE_COPY(URadioButtonPrivate)
    URadioButton* const q_ptr = nullptr;

    bool tristate = true;
    Qt::CheckState state = Qt::CheckState::Unchecked;
};

URadioButton::URadioButton(QWidget* parent) : QRadioButton(parent)
    , d_ptr(new URadioButtonPrivate(this))
{
    connect(this, &URadioButton::toggled, this, [ = ](bool checked)
    {
        Qt::CheckState state = checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
        setCheckState(state);
    });
}

URadioButton::URadioButton(const QString &text, QWidget* parent) : QRadioButton(text, parent)
    , d_ptr(new URadioButtonPrivate(this))
{
    connect(this, &URadioButton::toggled, this, [ = ](bool checked)
    {
        Qt::CheckState state = checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
        setCheckState(state);
    });
}

URadioButton::~URadioButton()
{
}

void URadioButton::setTristate(bool y)
{
    Q_D(URadioButton);
    if (d->tristate != y)
    {
        d->tristate = y;
        update();
    }
}

bool URadioButton::isTristate() const
{
    Q_D(const URadioButton);
    return d->tristate;
}

Qt::CheckState URadioButton::checkState() const
{
    Q_D(const URadioButton);
    return d->state;
}

void URadioButton::setCheckState(Qt::CheckState state)
{
    Q_D(URadioButton);
    if (d->state != state)
    {
        d->state = state;
        update();
        emit stateChanged(d->state);
    }
}

void URadioButton::paintEvent(QPaintEvent* event)
{
    UStylePainter painter(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    painter.drawControl(UProxyStyle::UControlElement::CE_RadioButton, opt);
}

void URadioButton::initStyleOption(QStyleOptionButton* button) const
{
    Q_D(const URadioButton);
    QRadioButton::initStyleOption(button);
    if (d->tristate && (d->state == Qt::CheckState::PartiallyChecked))
        button->state |= QStyle::State_NoChange;
}
