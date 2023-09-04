#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

class TitleBar;

class MessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);

    void setTitleIcon(const QIcon &icon);
    void setMessageBoxTitle(const QString &title);
    void setTextFont(const QFont &font);
    void setTextWordWrap(bool wordWrap);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:

private:
    TitleBar *titleBar = Q_NULLPTR;
    int margin = 8;
};

#endif // MESSAGEBOX_H
