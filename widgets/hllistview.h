#ifndef HLLISTVIEW_H
#define HLLISTVIEW_H

#include <QListView>

class HLListView : public QListView
{
    Q_OBJECT
public:
    explicit HLListView(QWidget *parent = nullptr);
    ~HLListView() Q_DECL_OVERRIDE;
    // 更新控件位置
    void updateContent();

    void initListView();

    void setRadius(int radius);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_raduis = 8;
};

#endif // HLLISTVIEW_H
