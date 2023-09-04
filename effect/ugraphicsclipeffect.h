#ifndef UGRAPHICSCLIPEFFECT_H
#define UGRAPHICSCLIPEFFECT_H

#include <QGraphicsEffect>
#include <QPainterPath>

class UGraphicsClipEffect : public QGraphicsEffect
{
    Q_OBJECT

    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath NOTIFY clipPathChanged)

public:
    explicit UGraphicsClipEffect(QObject* parent = Q_NULLPTR);

    QMargins margins() const;
    QPainterPath clipPath() const;

public Q_SLOTS:
    void setMargins(const QMargins &margins);
    void setClipPath(const QPainterPath &clipPath);

Q_SIGNALS:
    void marginsChanged(QMargins margins);
    void clipPathChanged(QPainterPath clipPath);

protected:
    void draw(QPainter* painter) Q_DECL_OVERRIDE;

private:
    QPainterPath m_clipPath;
    QMargins m_margins;
};

#endif // UGRAPHICSCLIPEFFECT_H
