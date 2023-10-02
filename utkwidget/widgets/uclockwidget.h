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
#ifndef UCLOCKWIDGET_H
#define UCLOCKWIDGET_H

#include "utkwidget_export.h"

#include <QWidget>

class UClockWidgetPrivate;
class UTKWIDGET_EXPORT UClockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UClockWidget(QWidget *parent = nullptr);
    ~UClockWidget() override;

    void setRadius(int radius);

    void startTime();

    void stopTime();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<UClockWidgetPrivate> d_ptr;
    Q_DISABLE_COPY(UClockWidget)
    Q_DECLARE_PRIVATE_D(d_ptr, UClockWidget)
};

#endif // UCLOCKWIDGET_H
