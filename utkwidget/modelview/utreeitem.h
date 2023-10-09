#ifndef UTREEITEM_H
#define UTREEITEM_H

#include "utkwidget_export.h"

#include <QIcon>
#include <QList>
#include <QVariant>
#include <QVector>

class UTreeModel;
class UTKWIDGET_EXPORT UTreeItem
{
public:
    explicit UTreeItem(const QVector<QVariant> &data, UTreeItem* parent = nullptr);
    ~UTreeItem();

    UTreeItem* child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    UTreeItem* parent() const;
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    void appendChild(UTreeItem* item);
    void appendChildren(QList<UTreeItem*> items);
    void insertChild(int position, UTreeItem* item);
    void insertChildren(int position, QList<UTreeItem*> items);

    void removeAllChildren();

    inline QIcon icon() const
    {
        return m_icon;
    }

    void setIcon(const QIcon &icon);

    int depth() const;

    QVector<QVariant> allData() const;

    void sortChildren(int column, Qt::SortOrder order, UTreeModel* model);

    void setParent(UTreeItem* parent);

    static bool isVariantLessThan(const QVariant &left, const QVariant &right,
                                  bool isLocaleAware = true, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

private:
    QList<UTreeItem*> childItems;
    QVector<QVariant> itemData;
    UTreeItem* parentItem;
    QIcon m_icon = QIcon();
};

#endif // UTREEITEM_H
