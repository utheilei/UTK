/*
* Copyright (C) 2023 ~ 2033 Ltd.
*
* Author:     helei <2454264168@qq.com>
*
* Maintainer: helei <2454264168@qq.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* General Public License version 3 as published by the Free Software
* Foundation and appearing in the file LICENSE.MIT included in the
* packaging of this file.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef UGAUGESPEED_H
#define UGAUGESPEED_H

#include "utkwidget_export.h"

#include <QWidget>

class UGaugeSpeedPrivate;
class UTKWIDGET_EXPORT UGaugeSpeed : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int radius READ getRadius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(double value READ getValue WRITE setValue NOTIFY valueChanged)
public:
    explicit UGaugeSpeed(QWidget *parent = nullptr);
    ~UGaugeSpeed() override;

    int getRadius();

    double getValue();

protected:
    void paintEvent(QPaintEvent *) override;

signals:
    void radiusChanged();

    void valueChanged();

public slots:
    void setRadius(int radius);   //设置表盘大小

    void setValue(int value);     //设置表盘显示值

private:
    QScopedPointer<UGaugeSpeedPrivate> d_ptr;
    Q_DISABLE_COPY(UGaugeSpeed)
    Q_DECLARE_PRIVATE_D(d_ptr, UGaugeSpeed)
};

#endif // UGAUGESPEED_H
