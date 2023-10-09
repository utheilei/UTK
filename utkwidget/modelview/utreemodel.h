#ifndef UTREEMODEL_H
#define UTREEMODEL_H

#include "utkwidget_export.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class UTreeItem;
class UTKWIDGET_EXPORT UTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    UTreeModel(const QStringList &headers, QObject* parent = Q_NULLPTR);
    ~UTreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    UTreeItem* getItem(const QModelIndex &index) const;

    void insertItem(int position, UTreeItem* item, bool isOk = true);

    void insertItems(int position, QList<UTreeItem*> items);

    void appendItem(UTreeItem* item, bool isOk = true);

    void appendItems(QList<UTreeItem*> items);

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    UTreeItem* rootItem() const;

    QModelIndex createTreeIndex(int row, int column, UTreeItem* parent) const;

    void changeTreePersistentIndexList(const QModelIndexList &from, const QModelIndexList &to);

    UTreeItem* itemFromIndex(const QModelIndex &index) const;

    QModelIndex indexFromItem(const UTreeItem* item) const;

    QList<UTreeItem*> findItems(const QString &text, Qt::SortOrder order);

    QModelIndexList findModelIndexs(const QString &text, Qt::SortOrder order);

    void clearModel();

private:
    QList<UTreeItem*> matchItems(UTreeItem* item, const QString &text);

signals:
    void rowCountChanged(int count);

private:
    UTreeItem* m_rootItem;
};

#endif // UTREEMODEL_H
