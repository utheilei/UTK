/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    gui.pageWidgetByObjectName("LicenseAgreementPage").entered.connect(changeLicenseLabels);
}


changeLicenseLabels = function()
{
    page = gui.pageWidgetByObjectName("LicenseAgreementPage");
    page.AcceptLicenseLabel.setText("Yes I do!");
    page.RejectLicenseLabel.setText("No I don't!");
}


Component.prototype.createOperations = function()
{
    //调用默认实现
    component.createOperations();
    
    if (systemInfo.productType === "windows") {
    	//添加桌面快捷方式
        component.addOperation("CreateShortcut",
                            "@TargetDir@/CADTools.exe",
                            "@DesktopDir@/CADTools.lnk",
                            "workingDirectory=@TargetDir@");

		//添加开始菜单快捷方式
        component.addOperation("CreateShortcut",
                            "@TargetDir@/CADTools.exe",
                            "@StartMenuDir@/CADTools.lnk",
                            "workingDirectory=@TargetDir@");
    }
}
