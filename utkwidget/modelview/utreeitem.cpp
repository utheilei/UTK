#include "utreeitem.h"
#include "utreemodel.h"

#include <QStringList>
#include <QDateTime>
#include <QModelIndexList>

UTreeItem::UTreeItem(const QVector<QVariant> &data, UTreeItem* parent)
{
    parentItem = parent;
    itemData = data;
}

UTreeItem::~UTreeItem()
{
    removeAllChildren();
}

UTreeItem* UTreeItem::child(int number)
{
    return childItems.at(number);
}

int UTreeItem::childCount() const
{
    return childItems.count();
}

int UTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<UTreeItem*>(this));

    return 0;
}

int UTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant UTreeItem::data(int column) const
{
    if (column > itemData.size() - 1)
        return QVariant();

    return itemData.value(column);
}

bool UTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        UTreeItem* item = new UTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool UTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (UTreeItem* child, childItems)
        child->insertColumns(position, columns);

    return true;
}

UTreeItem* UTreeItem::parent() const
{
    return parentItem;
}

bool UTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool UTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (UTreeItem* child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool UTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

void UTreeItem::appendChild(UTreeItem* item)
{
    childItems.append(item);
}

void UTreeItem::appendChildren(QList<UTreeItem*> items)
{
    childItems.append(items);
}

void UTreeItem::insertChild(int position, UTreeItem* item)
{
    if (position < 0 || position > childItems.size())
        return;

    childItems.insert(position, item);
}

void UTreeItem::insertChildren(int position, QList<UTreeItem*> items)
{
    if (position < 0 || position > childItems.size())
        return;

    int count = items.size();
    for (int row = position; row < count + position; ++row)
    {
        childItems.insert(row, items.at(row - position));
    }
}

void UTreeItem::removeAllChildren()
{
    if (!childItems.isEmpty())
    {
        qDeleteAll(childItems);
        childItems.clear();
    }
}

void UTreeItem::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

int UTreeItem::depth() const
{
    int d = 0;
    auto pNode = this;
    while (pNode->parent())
    {
        d++;
        pNode = pNode->parent();
    }

    return d;
}

QVector<QVariant> UTreeItem::allData() const
{
    return itemData;
}

void UTreeItem::sortChildren(int column, Qt::SortOrder order, UTreeModel* model)
{
    if (column >= columnCount() || childCount() <= 0)
        return;

    QVector<QPair<UTreeItem*, int>> sortable;
    sortable.reserve(childCount());

    for (int row = 0; row < childCount(); ++row)
    {
        UTreeItem* itm = child(row);
        if (itm)
            sortable.append(qMakePair(itm, row));
    }

    if (order == Qt::AscendingOrder)
    {
        std::stable_sort(sortable.begin(), sortable.end(), [column](const QPair<UTreeItem*, int> &s1, const QPair<UTreeItem*, int> &s2)
        {
            QVariant variant1 = s1.first->data(column);
            QVariant variant2 = s2.first->data(column);
            return isVariantLessThan(variant1, variant2);
        });
    }
    else
    {
        std::stable_sort(sortable.begin(), sortable.end(), [column](const QPair<UTreeItem*, int> &s1, const QPair<UTreeItem*, int> &s2)
        {
            QVariant variant1 = s1.first->data(column);
            QVariant variant2 = s2.first->data(column);
            return isVariantLessThan(variant2, variant1);
        });
    }

    QModelIndexList changedPersistentIndexesFrom, changedPersistentIndexesTo;
    QList<UTreeItem*> sorted_children;
    for (int i = 0; i < childCount(); ++i)
    {
        int r = sortable.at(i).second;
        UTreeItem* itm = child(r);
        sorted_children.append(itm);
        if (model)
        {
            QModelIndex from = model->createTreeIndex(r, 0, this);
            QModelIndex to = model->createTreeIndex(i, 0, this);
            changedPersistentIndexesFrom.append(from);
            changedPersistentIndexesTo.append(to);
        }
    }

    childItems = sorted_children;

    if (model)
    {
        model->changeTreePersistentIndexList(changedPersistentIndexesFrom, changedPersistentIndexesTo);
    }

    foreach (const auto &it, childItems)
    {
        it->sortChildren(column, order, model);
    }
}

void UTreeItem::setParent(UTreeItem* parent)
{
    parentItem = parent;
}

bool UTreeItem::isVariantLessThan(const QVariant &left, const QVariant &right, bool isLocaleAware, Qt::CaseSensitivity caseSensitivity)
{
    if (left.userType() == QVariant::Invalid)
        return false;
    if (right.userType() == QVariant::Invalid)
        return true;

    switch (left.userType())
    {
        case QVariant::Int:
            return left.toInt() < right.toInt();
        case QVariant::UInt:
            return left.toUInt() < right.toUInt();
        case QVariant::LongLong:
            return left.toLongLong() < right.toLongLong();
        case QVariant::ULongLong:
            return left.toULongLong() < right.toULongLong();
        case QMetaType::Float:
            return left.toFloat() < right.toFloat();
        case QVariant::Double:
            return left.toDouble() < right.toDouble();
        case QVariant::Char:
            return left.toChar() < right.toChar();
        case QVariant::Date:
            return left.toDate() < right.toDate();
        case QVariant::Time:
            return left.toTime() < right.toTime();
        case QVariant::DateTime:
            return left.toDateTime() < right.toDateTime();
        case QVariant::String:
        default:
            if (isLocaleAware)
                return left.toString().localeAwareCompare(right.toString()) < 0;
            else
                return left.toString().compare(right.toString(), caseSensitivity) < 0;
    }
}
