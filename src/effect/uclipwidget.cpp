#include "uclipwidget.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QPainterPath>

constexpr int radius = 8;

UClipWidget::UClipWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
    m_contentWidget = new QFrame(this);
    m_layout = new QVBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_contentWidget->setLayout(m_layout);

    // 矩形圆角
    m_clipEffectWidget = new UGraphicsClipEffect(m_contentWidget);
    m_contentWidget->installEventFilter(this);
    m_contentWidget->setGraphicsEffect(m_clipEffectWidget);

    mainLayout->addWidget(m_contentWidget);
    m_contentWidget->setAttribute(Qt::WA_TranslucentBackground);
}

UClipWidget::~UClipWidget()
{

}

void UClipWidget::setWidget(QWidget* widget)
{
    m_widget = widget;
    m_layout->addWidget(widget);
}

QWidget* UClipWidget::contentWidget() const
{
    return m_widget;
}

bool UClipWidget::eventFilter(QObject* watched, QEvent* event)
{
    Q_UNUSED(watched)

    if (event->type() != QEvent::Move && event->type() != QEvent::Resize)
        return false;

    QRect rect = m_contentWidget->rect();

    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    m_clipEffectWidget->setClipPath(path);

    return false;
}
