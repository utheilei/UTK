#include "titleswidget.h"
#include "widgets/delegate.h"

#include <QEvent>
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
        QObject::tr("Label"), QObject::tr("Button"), QObject::tr("LineEdit"), QObject::tr("datetime"),
        QObject::tr("developing"), QObject::tr("developing 1"), QObject::tr("developing 2"), QObject::tr("developing 3"),
        QObject::tr("developing 4"), QObject::tr("developing 5"), QObject::tr("developing 6"), QObject::tr("developing 7"),
        QObject::tr("developing 8"), QObject::tr("developing 9"), QObject::tr("developing 10")
    };

    const QList<QString> iconList =
    {
        "utk_label", "utk_button", "utk_lineedit", "utk_datetime",
        "utk_db", "utk_network", "utk_flag", "utk_lampbulb",
        "utk_message", "utk_notice", "utk_painter", "utk_settings",
        "utk_statistics", "utk_renew", "title"
    };

    for (int i = 0; i < textList.size(); i++)
    {
        addItem(QIcon::fromTheme(iconList.at(i)), textList[i]);
    }
}

void TitlesWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        const QVector<QString> textList =
        {
            QObject::tr("Label"), QObject::tr("Button"), QObject::tr("LineEdit"), QObject::tr("datetime"),
            QObject::tr("developing"), QObject::tr("developing 1"), QObject::tr("developing 2"), QObject::tr("developing 3"),
            QObject::tr("developing 4"), QObject::tr("developing 5"), QObject::tr("developing 6"), QObject::tr("developing 7"),
            QObject::tr("developing 8"), QObject::tr("developing 9"), QObject::tr("developing 10")
        };
        for (int i = 0; i < textList.size(); i++)
        {
            m_model->item(i, 0)->setText(textList[i]);
        }
    } else {
        HLListView::changeEvent(event);
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
