#ifndef HLTOOLTIP_H
#define HLTOOLTIP_H

#include <QWidget>

class QLabel;
class QHBoxLayout;
class UGraphicsGlowEffect;

class HLToolTip : public QWidget
{
    Q_OBJECT
public:
    explicit HLToolTip(QWidget *parent = nullptr);

    void setIcon(const QIcon &icon);
    void setText(const QString &text);
    void setRadius(int radius);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void addGraphicsEffect();

signals:

public slots:

private:
    QLabel *iconLabel = Q_NULLPTR;
    QLabel *textLabel = Q_NULLPTR;
    QHBoxLayout *layout = Q_NULLPTR;
    UGraphicsGlowEffect *graphicsClipEffect = Q_NULLPTR;
    int m_radius = 8;
};

#endif // HLTOOLTIP_H
