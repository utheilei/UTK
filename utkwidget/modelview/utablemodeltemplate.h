#ifndef UTABLEMODELTEMPLATE_H
#define UTABLEMODELTEMPLATE_H

#include "utkwidget_export.h"

#include <QAbstractItemModel>
#include <QDateTime>

#include <set>

template <class T>
class UTKWIDGET_EXPORT UTableModelTemplate : public QAbstractItemModel
{
public:
    explicit UTableModelTemplate(QObject* parent = Q_NULLPTR) : QAbstractItemModel(parent) {}
    ~UTableModelTemplate() Q_DECL_OVERRIDE {}

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(parent)

        if (row >= dataList.size())
            return QModelIndex();

        if (dataList.empty())
            return QModelIndex();

        if (!dataList.empty())
            return createIndex(row, column);

        return QModelIndex();
    }

    virtual QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(child)
        return QModelIndex();
    }

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE
    {
        return parent.isValid() ? 0 : dataList.size();
    }

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(parent)

        return headers.count();
    }

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE
    {
        if (Qt::Vertical == orientation)
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            if (section >= headers.size())
                return QVariant();

            return headers.at(section);
        }

        if (role == Qt::TextAlignmentRole)
            return alignmentFlag;

        return QVariant();
    }

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE
    {
        if (row < 0 || count < 1)
            return false;

        beginInsertRows(parent, row, row + count - 1);

        for (int i = row; i <= row + count - 1; ++i)
        {
            T record;
            dataList.insert(i, record);
        }

        endInsertRows();

        return true;
    }

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE
    {
        if (row < 0 || row >= dataList.size())
            return false;

        int romoveEnd = row + count > dataList.size() ? dataList.size() - 1 : row + count - 1;

        beginRemoveRows(parent, row, row + count - 1);

        for (int i = romoveEnd; i >= row; --i)
        {
            dataList.removeAt(i);
        }

        endRemoveRows();

        return true;
    }

    virtual void fetchMore(const QModelIndex &parent) Q_DECL_OVERRIDE
    {
        if (parent.isValid())
            return;

        int remainder = dataList.size() - singleLoadCount;
        int itemsToFetch = qMin(300, remainder);

        if (itemsToFetch <= 0)
            return;

        beginInsertRows(QModelIndex(), singleLoadCount, singleLoadCount + itemsToFetch - 1);

        singleLoadCount += itemsToFetch;

        endInsertRows();
    }

    virtual bool canFetchMore(const QModelIndex &parent) const Q_DECL_OVERRIDE
    {
        if (parent.isValid())
            return false;

        return (singleLoadCount < dataList.size());
    }

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE
    {
        beginResetModel();

        std::stable_sort(dataList.begin(), dataList.end(), [ = ](T first, T last)->bool {
            return sortByColumn(column, order, first, last);
        });

        endResetModel();
    }

public:
    QStringList headerList() const
    {
        return headers;
    }

    bool isEmpty() const
    {
        return (rowCount() <= 0);
    }

    void setItems(const QList<T> &list)
    {
        beginResetModel();
        dataList = list;
        endResetModel();
    }

    bool insertItem(int row, T node, const QModelIndex &parent = QModelIndex())
    {
        beginInsertRows(parent, row, row);

        if (row < 0)
        {
            dataList.push_front(node);
        }
        if (row >= dataList.size())
        {
            dataList.push_back(node);
        }
        else
        {
            dataList.insert(dataList.begin() + row, node);
        }

        endInsertRows();

        return true;
    }

    // 根据索引集合删除元素
    bool removeDatesByIndexs(std::set<int> indexs)
    {
        beginResetModel();
        for (auto iter = indexs.rbegin(); iter != indexs.rend(); ++iter)
        {
            dataList.removeAt(*iter);
        }
        endResetModel();

        return true;
    }

    bool clear()
    {
        beginRemoveRows(QModelIndex(), 0, dataList.size() - 1);
        dataList.clear();
        endRemoveRows();
    }

protected:
    virtual bool sortByColumn(int column, Qt::SortOrder order, T first, T last)
    {
        Q_UNUSED(column)
        Q_UNUSED(order)
        Q_UNUSED(first)
        Q_UNUSED(last)
    }

    bool isVariantLessThan(const QVariant &left, const QVariant &right,
                           bool isLocaleAware = true, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive)
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

protected:
    // 存放表头字段值，在子类中赋值
    QStringList headers;

    // 表头字段对齐方式，默认左对齐，可修改
    Qt::AlignmentFlag alignmentFlag = Qt::AlignLeft;

    QList<int> columnWidths;
    QList<T> dataList;
    int singleLoadCount = 0;
};

#endif // UTABLEMODELTEMPLATE_H

