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
#ifndef UWATERPROGRESS_H
#define UWATERPROGRESS_H

#include "utkwidget_export.h"

#include <QWidget>

class UWaterProgressPrivate;
class UTKWIDGET_EXPORT UWaterProgress : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double value READ getValue WRITE setValue)

public:
    explicit UWaterProgress(QWidget* parent = nullptr);
    ~UWaterProgress();

    double getValue() const;

    void start();

    void stop();

public Q_SLOTS:
    void setValue(double value);

    void setBeginColor(const QColor &beginColor);

    void setEndColor(const QColor &endColor);

protected:
    virtual void paintEvent(QPaintEvent*) override;

private:
    QScopedPointer<UWaterProgressPrivate> d_ptr;
    Q_DISABLE_COPY(UWaterProgress)
    Q_DECLARE_PRIVATE_D(d_ptr, UWaterProgress)
};

#endif // UWATERPROGRESS_H
