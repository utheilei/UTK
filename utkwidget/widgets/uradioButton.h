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
#ifndef URADIOBUTTON_H
#define URADIOBUTTON_H

#include "utkwidget_export.h"

#include <QRadioButton>

class URadioButtonPrivate;
class UTKWIDGET_EXPORT URadioButton : public QRadioButton
{
    Q_OBJECT

public:
    explicit URadioButton(QWidget* parent = nullptr);
    explicit URadioButton(const QString &text, QWidget* parent = nullptr);
    ~URadioButton();

    void setTristate(bool y = true);
    bool isTristate() const;

    Qt::CheckState checkState() const;
    void setCheckState(Qt::CheckState state);

Q_SIGNALS:
    void stateChanged(Qt::CheckState state);

protected:
    void paintEvent(QPaintEvent*) override final;
    void initStyleOption(QStyleOptionButton* button) const;

private:
    QScopedPointer<URadioButtonPrivate> d_ptr;
    Q_DISABLE_COPY(URadioButton)
    Q_DECLARE_PRIVATE_D(d_ptr, URadioButton)
};

#endif // URADIOBUTTON_H
