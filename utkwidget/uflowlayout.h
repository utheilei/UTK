#ifndef UFLOWLAYOUT_H
#define UFLOWLAYOUT_H

#include "utkwidget_export.h"

#include <QLayout>
#include <QRect>
#include <QStyle>

class UTKWIDGET_EXPORT UFlowLayout : public QLayout
{
public:
    explicit UFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit UFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~UFlowLayout();

    void addItem(QLayoutItem* item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem* takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem*> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // UFLOWLAYOUT_H
