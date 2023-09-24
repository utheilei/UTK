#include "titleswidget.h"
#include "widgets/delegate.h"

#include <QHBoxLayout>
#include <QStandardItemModel>

TitlesWidget::TitlesWidget(QWidget* parent)
    : HLListView(parent)
    , m_model(new QStandardItemModel(this))
    , m_itemDelegate(new TiTleStyledDelegate(this))
{
    setRadius(15);
    init();
    initItems();
}

void TitlesWidget::init()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setViewMode(ViewMode::IconMode);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setDragEnabled(false);
    setSpacing(20);
    setResizeMode(QListView::Adjust);
    setFrameShape(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setItemDelegate(m_itemDelegate);
    setModel(m_model);
}

void TitlesWidget::initItems()
{
    const QVector<QString> textList =
    {
        QObject::tr("Label"), QObject::tr("Button"), QObject::tr("LineEdit"), QObject::tr("defapp"),
        QObject::tr("personalization"), QObject::tr("network"), QObject::tr("sound"), QObject::tr("datetime"),
        QObject::tr("power"), QObject::tr("mouse"), QObject::tr("keyboard"), QObject::tr("unionid"),
        QObject::tr("update"), QObject::tr("systeminfo"), QObject::tr("commoninfo")
    };

    const QList<QString> iconList =
    {
        "dcc_nav_accounts", "dcc_nav_cloudsync", "dcc_nav_display", "dcc_nav_defapp",
        "dcc_nav_personalization", "dcc_nav_network", "dcc_nav_sound", "dcc_nav_datetime",
        "dcc_nav_power", "dcc_nav_mouse", "dcc_nav_keyboard", "dcc_nav_unionid",
        "dcc_nav_update", "dcc_nav_systeminfo", "dcc_nav_commoninfo"
    };

    for (int i = 0; i < textList.size(); i++)
    {
        addItem(QIcon::fromTheme("title"), textList[i]);
    }
}

void TitlesWidget::addItem(const QIcon &icon, const QString &text)
{
    QStandardItem* item = new QStandardItem();
    item->setText(text);
    item->setData(QVariant::fromValue(icon), Qt::DecorationRole);
    item->setData(m_size, Qt::SizeHintRole);
    m_model->appendRow(item);
}
