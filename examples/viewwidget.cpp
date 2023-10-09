#include "viewwidget.h"
#include "modelview/utreemodel.h"
#include "modelview/utreeitem.h"
#include "modelview/utreeview.h"

#include <QHBoxLayout>

ViewWidget::ViewWidget(QWidget* parent) : UWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setRadius(8);

    UTreeView* view = new UTreeView(this);
    QStringList headers;
    headers << tr("Title");
    UTreeModel* model = new UTreeModel(headers, view);
    view->setModel(model);

    QVector<QVariant> data = {"链路"};
    QVector<QVariant> data22 = {"控制器"};
    QVector<QVariant> inputData = {"输入"};
    QVector<QVariant> data1 = {"控制器：MX401 (192.168.2.1)"};
    QVector<QVariant> data2 = {"箱体：MX401 (192.168.2.2)"};
    QVector<QVariant> data3 = {"控制器：MX401 (192.168.2.3)"};
    QVector<QVariant> data4 = {"箱体：MX401 (192.168.2.4)"};
    for (int i = 0; i < 1; i++)
    {
        UTreeItem* item = new UTreeItem(data, model->rootItem());
        UTreeItem* item1 = new UTreeItem(data22, model->rootItem());
        UTreeItem* item2 = new UTreeItem(inputData, model->rootItem());
        item->appendChild(new UTreeItem(data1, item));
        item->appendChild(new UTreeItem(data2, item));
        item1->appendChild(new UTreeItem(data3, item1));
        item1->appendChild(new UTreeItem(data4, item1));
        model->appendItem(item);
        model->appendItem(item1);
        model->appendItem(item2);
    }

    view->expandAll();

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    setLayout(mainLayout);
}

ViewWidget::~ViewWidget()
{
}
