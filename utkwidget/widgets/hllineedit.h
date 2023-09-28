#ifndef HLLINEEDIT_H
#define HLLINEEDIT_H

#include "utkwidget_export.h"

#include <QLineEdit>
#include <QPushButton>

class LineEditButton : public QPushButton
{
public:
    LineEditButton(QWidget* parent = Q_NULLPTR);

    void setRadius(int radius);

    void setBackGroundColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    QColor m_color = QColor();
    int m_radius = 8;
};

class QLabel;
class ToolTip;
class UTKWIDGET_EXPORT HLLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HLLineEdit(QWidget* parent = nullptr);
    explicit HLLineEdit(const QString &text, QWidget* parent = nullptr);
    ~HLLineEdit() override;

    void setRadius(int radius);

    bool closeButtonVisible();
    //    void setCloseButtonVisible(bool isVisible);

    void addLeftAction(const QPixmap &pixmap);

    void addLeftAction(const QString &text);

    void clearAction();

    bool isAlert();

    void showAlertMessage(const QString &text, int duration = 3000);

    void hideAlertMessage();

private:
    void initLineEdit();
    void initToolTip();
    void initConnection();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

signals:

public slots:

private:
    int m_radius = 8;
    bool m_isAlert = false;
    LineEditButton* closeButton = nullptr;
    QLabel* label = nullptr;
    ToolTip* toolTip = nullptr;
};

#endif // HLLINEEDIT_H
