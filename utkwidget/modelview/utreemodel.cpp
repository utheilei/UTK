#include "utreeitem.h"
#include "utreemodel.h"

UTreeModel::UTreeModel(const QStringList &headers, QObject* parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    m_rootItem = new UTreeItem(rootData);
}

UTreeModel::~UTreeModel()
{
    delete m_rootItem;
}

int UTreeModel::columnCount(const QModelIndex &) const
{
    return m_rootItem->columnCount();
}

QVariant UTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    UTreeItem* item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags UTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

UTreeItem* UTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        UTreeItem* item = static_cast<UTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

void UTreeModel::insertItem(int position, UTreeItem* item, bool isOk)
{
    if (isOk)
    {
        beginInsertRows(QModelIndex(), position, position);
        if (m_rootItem)
            m_rootItem->insertChild(position, item);
        endInsertRows();
    }
}

void UTreeModel::insertItems(int position, QList<UTreeItem*> items)
{
    int count = items.size();
    if (count <= 0)
        return;

    beginInsertRows(QModelIndex(), position, position + count - 1);
    m_rootItem->insertChildren(position, items);
    endInsertRows();

    emit rowCountChanged(rowCount());
}

void UTreeModel::appendItem(UTreeItem* item, bool isOk)
{
    if (item == nullptr)
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    if (m_rootItem && isOk)
    {
        m_rootItem->appendChild(item);
        emit rowCountChanged(rowCount());
    }
    endInsertRows();
}

void UTreeModel::appendItems(QList<UTreeItem*> items)
{
    int count = items.size();
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + count - 1);
    if (m_rootItem)
    {
        m_rootItem->appendChildren(items);
        emit rowCountChanged(rowCount());
    }
    endInsertRows();
}

void UTreeModel::sort(int column, Qt::SortOrder order)
{
    m_rootItem->sortChildren(column, order, this);
}

UTreeItem* UTreeModel::rootItem() const
{
    return m_rootItem;
}

QModelIndex UTreeModel::createTreeIndex(int row, int column, UTreeItem* parent) const
{
    return createIndex(row, column, parent);
}

void UTreeModel::changeTreePersistentIndexList(const QModelIndexList &from, const QModelIndexList &to)
{
    changePersistentIndexList(from, to);
}

UTreeItem* UTreeModel::itemFromIndex(const QModelIndex &index) const
{
    auto model = index.model();
    if (nullptr == model)
        return nullptr;

    if (!index.isValid())
        return m_rootItem;

    UTreeItem* item = static_cast<UTreeItem*>(index.internalPointer());

    return item;
}

QModelIndex UTreeModel::indexFromItem(const UTreeItem* item) const
{
    if (item && item->parent())
    {
        return createIndex(item->childNumber(), 0, const_cast<UTreeItem*>(item));
    }

    return QModelIndex();
}

QList<UTreeItem*> UTreeModel::findItems(const QString &text, Qt::SortOrder order)
{
    QList<UTreeItem*> items = matchItems(m_rootItem, text);

    std::stable_sort(items.begin(), items.end(), [order, this](const UTreeItem * infoA, const UTreeItem * infoB)->bool
    {
        if (Qt::SortOrder::AscendingOrder == order)
        {
            return indexFromItem(infoA).row() < indexFromItem(infoB).row();
        }
        else
        {
            return indexFromItem(infoA).row() > indexFromItem(infoB).row();
        }
    });

    return items;
}

QModelIndexList UTreeModel::findModelIndexs(const QString &text, Qt::SortOrder order)
{
    QModelIndexList list;
    QList<UTreeItem*> items = findItems(text, order);

    foreach (const auto &item, items)
        list.append(indexFromItem(item));

    return list;
}

void UTreeModel::clearModel()
{
    beginResetModel();
    m_rootItem->removeAllChildren();
    endResetModel();
}

QList<UTreeItem*> UTreeModel::matchItems(UTreeItem* item, const QString &text)
{
    QList<UTreeItem*> items;

    if (nullptr == item)
        return items;

    for (int i = 0; i < item->childCount(); i++)
    {
        auto childItem = item->child(i);
        if (nullptr == childItem)
            continue;

        if (childItem->childCount() <= 0)
        {
            if (0 == QString::compare(text, childItem->data(2).toString()))
                items.append(childItem);
        }
        else
        {
            items.append(matchItems(childItem, text));
        }
    }

    return items;
}

QVariant UTreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex UTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    UTreeItem* parentItem = getItem(parent);

    UTreeItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool UTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = m_rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool UTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    UTreeItem* parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, m_rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex UTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    UTreeItem* childItem = getItem(index);
    UTreeItem* parentItem = childItem->parent();

    if (parentItem == m_rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool UTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = m_rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (m_rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool UTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    UTreeItem* parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int UTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) return 0;

    UTreeItem* parentItem = getItem(parent);

    return parentItem->childCount();
}

bool UTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    UTreeItem* item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {role});

    return result;
}

bool UTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                               const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = m_rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}
