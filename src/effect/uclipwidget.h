/*
* Copyright (C) 2023 ~ 2026.
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
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include "utkwidget_export.h"
#include "ugraphicsclipeffect.h"

#include <QFrame>

class QVBoxLayout;
class UTKWIDGET_EXPORT UClipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UClipWidget(QWidget* parent = nullptr);
    ~UClipWidget() Q_DECL_OVERRIDE;

    void setWidget(QWidget* widget);
    QWidget* contentWidget() const;

protected:
    bool eventFilter(QObject* watched, QEvent* event) Q_DECL_OVERRIDE;

private:
    UGraphicsClipEffect* m_clipEffectWidget = Q_NULLPTR;
    QFrame* m_contentWidget;
    QVBoxLayout* m_layout;
    QWidget* m_widget = Q_NULLPTR;
};
#endif // SETTINGWIDGET_H
