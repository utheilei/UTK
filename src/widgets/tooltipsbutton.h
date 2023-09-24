#ifndef TOOLTIPSBUTTON_H
#define TOOLTIPSBUTTON_H

#include "utkwidget_export.h"

#include <QWidget>
#include <QTimer>

class QLabel;
class QBoxLayout;
class QMovie;

class UTKWIDGET_EXPORT ToolTipsButton : public QWidget
{
    Q_OBJECT
public:
    explicit ToolTipsButton(QWidget* parent = Q_NULLPTR, Qt::Orientation orientation = Qt::Vertical);

    void setText(const QString &text);

    void setIconSize(const QSize &size);

    void setIcon(const QIcon &icon);

    void setSpacing(int space);

    void setMovie(const QString &filePath);

    void startMovie();

    void stopMovie();

    void startGif();

    void stopGif();

    void setIconLabelVisible(bool isVisible);

    void setTextLabelVisible(bool isVisible);

signals:

public slots:

private:
    QLabel* m_iconLabel = Q_NULLPTR;
    QLabel* m_textLabel = Q_NULLPTR;
    QSize m_size = QSize(20, 20);
    QBoxLayout* m_mainLayout = Q_NULLPTR;
    QMovie* m_pMovie = Q_NULLPTR;
    QTimer timer;
    int count = 0;
};

#endif // TOOLTIPSBUTTON_H
