#include "utreeview.h"

#include <QHeaderView>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>

constexpr int iconWidth = 15;

class UTreeViewPrivate
{
public:
    UTreeViewPrivate(UTreeView* q) : q_ptr(q) {}
    ~UTreeViewPrivate() {}

    void init();

private:
    void initUi();

private:
    Q_DECLARE_PUBLIC(UTreeView)
    UTreeView* const q_ptr = Q_NULLPTR;
};

void UTreeViewPrivate::init()
{
    initUi();
}

void UTreeViewPrivate::initUi()
{
    Q_Q(UTreeView);

    q->setMouseTracking(true);
    q->viewport()->setMouseTracking(true);
    //    TreeViewDelegate *delegate = new TreeViewDelegate(q);
    //    q->setItemDelegate(delegate);

    q->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    q->setFrameShape(QFrame::NoFrame);
    q->setSelectionMode(QAbstractItemView::SingleSelection);
    q->setSelectionBehavior(QAbstractItemView::SelectRows);
    q->setEditTriggers(QAbstractItemView::NoEditTriggers);
    q->header()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    q->setTextElideMode(Qt::TextElideMode::ElideMiddle);
    q->header()->setDefaultSectionSize(200);
    q->header()->setStretchLastSection(true);
    q->header()->setSectionsMovable(false);
    q->header()->setVisible(false);
    q->setSortingEnabled(true);
    q->viewport()->setAutoFillBackground(true);
}

UTreeView::UTreeView(QWidget* parent)
    : QTreeView(parent)
    , d_ptr(new UTreeViewPrivate(this))
{
    Q_D(UTreeView);

    d->init();
}

UTreeView::~UTreeView()
{
    Q_D(UTreeView);

    delete d;
}

void UTreeView::keyPressEvent(QKeyEvent* event)
{
    QTreeView::keyPressEvent(event);

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
        emit clicked(currentIndex());
}

void UTreeView::drawBranches(QPainter* painter, const QRect &rect, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setOpacity(1);

    auto model = index.model();
    if (nullptr == model)
        return;

    bool hasChild = model->rowCount(index) > 0;
    bool expaned = isExpanded(index);

    QPalette palette = this->palette();
    QBrush background = palette.color(QPalette::Base);

    QPainterPath path;
    path.addRect(rect);
    painter->fillPath(path, background);

    QIcon icon;
    if (expaned)
    {
        if (hasChild)
            icon = QIcon::fromTheme("utk_expand");
    }
    else
    {
        if (hasChild)
            icon = QIcon::fromTheme("utk_collapse");
    }

    if (!icon.isNull())
    {
        icon.paint(painter, rect, Qt::AlignCenter);
    }

    painter->restore();
}

void UTreeView::updateContent()
{
    updateGeometries();
}
