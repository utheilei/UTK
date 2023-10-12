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
#ifndef USWITCHBUTTON_H
#define USWITCHBUTTON_H

#include "utkwidget_export.h"

#include <QAbstractButton>

class USwitchButtonPrivate;
class UTKWIDGET_EXPORT USwitchButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(int offset READ offset WRITE setOffset)

public:
    explicit USwitchButton(QWidget* parent = nullptr, bool bEnergtic = true);
    ~USwitchButton() override;

    virtual qreal margin();
    void setMargin(qreal margin);

    Qt::CheckState checkState() const;
    bool isTristate() const;
    void setCheckState(Qt::CheckState state);
    void setTristate(bool y = true);
    void setCheckText(const QString &text);
    void setNoCheckText(const QString &text);

signals:
    void stateChanged(int state);

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
    virtual void checkStateSet() override;

    int offset();
    void setOffset(int offset);

private:
    QScopedPointer<USwitchButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(USwitchButton)
    Q_DISABLE_COPY(USwitchButton)
};

#endif // USWITCHBUTTON_H
