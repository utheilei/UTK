#include "hllistview.h"
#include "delegate.h"

#include <QPainter>
#include <QPainterPath>

HLListView::HLListView(QWidget *parent) : QListView(parent)
{
    auto viewportPalette = viewport()->palette();
    viewportPalette.setBrush(QPalette::Base, Qt::transparent);
    viewport()->setPalette(viewportPalette);
}

HLListView::~HLListView()
{
}

void HLListView::initListView()
{
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    StyledDelegate *delegate = new StyledDelegate(this);
    setItemDelegate(delegate);

    setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setTextElideMode(Qt::TextElideMode::ElideMiddle);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QListView::SingleSelection);
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    setSpacing(5);

//    setDragEnabled(true);
//    setDragDropMode(DragDropMode::InternalMove);
    //    setDropIndicatorShown(true);
}

void HLListView::setRadius(int radius)
{
    if (m_raduis != radius) {
        m_raduis = radius;
    }
}

void HLListView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this->viewport());
    painter.setRenderHints(painter.renderHints()
                            | QPainter::Antialiasing
                            | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    rectPath.addRoundedRect(this->rect(), m_raduis, m_raduis);
    painter.fillPath(rectPath, palette().color(QPalette::Base));

    QListView::paintEvent(event);

    QModelIndex index = indexAt(mapFromGlobal(this->cursor().pos()));

    if (index.isValid() && showDropIndicator() && state() == QAbstractItemView::State::DraggingState
            && viewport()->cursor().shape() != Qt::ForbiddenCursor) {
        painter.save();
        QRect rect = visualRect(index);
        rect = rect.adjusted(1, 1, -1, -1);
        painter.setPen(QPen(palette().color(QPalette::Highlight), 2));
        painter.setBrush(Qt::NoBrush);
        if (DropIndicatorPosition::OnItem == dropIndicatorPosition()) {
            QPainterPath path;
            path.addRoundedRect(rect, m_raduis, m_raduis);
            painter.drawPath(path);
        } else if (DropIndicatorPosition::AboveItem == dropIndicatorPosition()) {
            painter.drawLine(QLine(rect.topLeft(), rect.topRight()));
        } else if (DropIndicatorPosition::BelowItem == dropIndicatorPosition()) {
            painter.drawLine(QLine(rect.bottomLeft(), rect.bottomRight()));
        }

        painter.restore();
    }
}
