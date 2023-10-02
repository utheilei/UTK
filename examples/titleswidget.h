#ifndef TITLESWIDGET_H
#define TITLESWIDGET_H

#include "widgets/hllistview.h"

class TiTleStyledDelegate;
class QStandardItemModel;
class TitlesWidget : public HLListView
{
    Q_OBJECT
public:
    explicit TitlesWidget(QWidget *parent = nullptr);

    void addItem(const QIcon &icon, const QString &text);

protected:
    void init();
    void initItems();

    void changeEvent(QEvent *event) override;

private:
    QStandardItemModel *m_model = Q_NULLPTR;
    TiTleStyledDelegate *m_itemDelegate = Q_NULLPTR;
    QSize m_size = QSize(170, 170);
};

#endif // TITLESWIDGET_H
