#include "messagebox.h"
#include "titlebar.h"

#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QIcon>
#include <QLabel>

//    MessageBox customMsgBox;
//    customMsgBox.setWindowTitle(tr("关于本软件"));
//    StyleButton* styleButton = new StyleButton(tr("好的"), this);
//    styleButton->setRadius(8);
//    customMsgBox.addButton(styleButton, QMessageBox::ActionRole);
//    customMsgBox.setIconPixmap(QPixmap(":/images/about.png").scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//    customMsgBox.setText(tr("欢迎使用Qt-Demo软件"));
//    customMsgBox.setTextFont(QFont("SimHei", 10));
//    customMsgBox.setTextWordWrap(true);
//    customMsgBox.exec();

MessageBox::MessageBox(QWidget* parent) : QMessageBox(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    
    titleBar = new UTitleBar(this);
    titleBar->setMenuVisible(false);
    titleBar->setMinButtonVisible(false);
    titleBar->setMaxButtonVisible(false);
    QMargins margins = contentsMargins();
    setContentsMargins(margins.left() + margin, titleBar->height() + margin,
                       margins.right() + margin, margins.bottom() + margin);
    titleBar->setTitleBarIcon(qApp->windowIcon());
    
    connect(titleBar, &UTitleBar::closeButtonClicked, this, &MessageBox::reject);
}

void MessageBox::setTitleIcon(const QIcon &icon)
{
    titleBar->setTitleBarIcon(icon);
}

void MessageBox::setMessageBoxTitle(const QString &title)
{
    titleBar->setWindowTitle(title);
}

void MessageBox::setTextFont(const QFont &font)
{
    auto label = findChild<QLabel*>(QLatin1String("qt_msgbox_label"));
    if (label)
        label->setFont(font);
}

void MessageBox::setTextWordWrap(bool wordWrap)
{
    auto label = findChild<QLabel*>(QLatin1String("qt_msgbox_label"));
    if (label)
        label->setWordWrap(wordWrap);
}

void MessageBox::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    QRect contentRect(rect().topLeft().x() + margin, rect().topLeft().y() + margin,
                      (rect().width() - 2 * margin), (rect().height() - 2 * margin));
    rectPath.addRoundedRect(contentRect, 15, 15);
    painter.fillPath(rectPath, palette().color(QPalette::Base));

    QColor color(92, 93, 95, 50);
    int arr[8] = {10, 20, 30, 40, 50, 80, 120, 150};
    for (int i = 0; i < margin; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(rect().topLeft().x() + i, rect().topLeft().y() + i,
                            (rect().width() - 2 * i), (rect().height() - 2 * i), 15, 15);

        color.setAlpha(arr[i]);
        painter.setPen(color);
        painter.drawPath(path);
    }

    QMessageBox::paintEvent(event);
}

void MessageBox::resizeEvent(QResizeEvent* event)
{
    titleBar->setGeometry(margin, margin, this->width() - margin * 2, titleBar->height());
    QMessageBox::resizeEvent(event);
}
