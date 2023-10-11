#include "viewwidget.h"
#include "modelview/utreemodel.h"
#include "modelview/utreeitem.h"
#include "modelview/utreeview.h"

#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>

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
        item->setIcon(QIcon::fromTheme("utk_notice"));
        item->setChecked(true);
        UTreeItem* item1 = new UTreeItem(data22, model->rootItem());
        item1->setIcon(QIcon::fromTheme("utk_notice"));
        UTreeItem* item2 = new UTreeItem(inputData, model->rootItem());
        item2->setIcon(QIcon::fromTheme("utk_notice"));
        item->appendChild(new UTreeItem(data1, item));
        item->appendChild(new UTreeItem(data2, item));
        item1->appendChild(new UTreeItem(data3, item1));
        item1->appendChild(new UTreeItem(data4, item1));
        model->appendItem(item);
        model->appendItem(item1);
        model->appendItem(item2);
    }

    view->expandAll();

    UTreeView* tableView = new UTreeView(this);
//    tableView->header()->setVisible(true);
    tableView->setRootIsDecorated(false);
    QStandardItemModel *standardItemModel = new QStandardItemModel(tableView);
    tableView->setModel(standardItemModel);
    QStringList list = {"1", "2", "3"};
    standardItemModel->setHorizontalHeaderLabels(list);
    for (int var = 0; var < 10; ++var) {
        QStandardItem *item = new QStandardItem(QString("QStandardItem%1").arg(var));
        QStandardItem *item1 = new QStandardItem("b");
        QStandardItem *item2 = new QStandardItem("c");
        QList<QStandardItem*> list = {item, item1, item2};
        standardItemModel->appendRow(list);
    }

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);
}

ViewWidget::~ViewWidget()
{
}
