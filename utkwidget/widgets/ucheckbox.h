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
#ifndef UCHECKBOX_H
#define UCHECKBOX_H

#include "utkwidget_export.h"

#include <QCheckBox>

class UCheckBoxPrivate;
class UTKWIDGET_EXPORT UCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    explicit UCheckBox(QWidget* parent = nullptr);
    explicit UCheckBox(const QString &text, QWidget* parent = nullptr);
    ~UCheckBox();

protected:
    void paintEvent(QPaintEvent*) override final;

private:
    QScopedPointer<UCheckBoxPrivate> d_ptr;
    Q_DISABLE_COPY(UCheckBox)
    Q_DECLARE_PRIVATE_D(d_ptr, UCheckBox)
};

#endif // UCHECKBOX_H
