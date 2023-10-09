#ifndef UTREEVIEW_H
#define UTREEVIEW_H

#include "utkwidget_export.h"

#include <QTreeView>

class UTreeViewPrivate;
class UTKWIDGET_EXPORT UTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit UTreeView(QWidget* parent = Q_NULLPTR);
    ~UTreeView() Q_DECL_OVERRIDE;

    void updateContent();

protected:
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void drawBranches(QPainter* painter, const QRect &rect, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(UTreeView)
    Q_DECLARE_PRIVATE(UTreeView)

    UTreeViewPrivate* d_ptr = Q_NULLPTR;
};

#endif // UTREEVIEW_H
