#ifndef ARROWCIRCLEBUTTON_H
#define ARROWCIRCLEBUTTON_H

#include <QAbstractButton>

#define PIX_SIZE 60

class ArrowCircleButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit ArrowCircleButton(QWidget *parent = Q_NULLPTR);

    void setSelected(bool selected);

    void setArrowed(bool arrowed);

    inline bool arrowed() const { return m_arrowed; }

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;

private:
    bool m_hover = false;
    bool m_selected = false;
    bool m_arrowed = false;
};

#endif // ARROWCIRCLEBUTTON_H
