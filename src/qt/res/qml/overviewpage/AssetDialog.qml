/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0

Dialog {
	id: dialog
    visible: false
    title: "Blue sky dialog"
	modality: Qt.ApplicationModal
	standardButtons: Dialog.NoButton	//StandardButton.Ok|StandardButton.Cancel

	signal finished(string name, string balance, string unconfirmed_balance, string total_balance)

	GridLayout {
	
		id: form
		property alias name: name
		property alias balance: balance
		property alias unconfirmed_balance: unconfirmed_balance
		property alias total_balance: total_balance
		property int minimumInputSize: 12
		property string placeholderText: qsTr("<enter>")	

		
		rows: 7
		columns: 2
		implicitWidth: 300
		implicitHeight: 100
			
		Label {
			x: 0
			y: 15
			text: qsTr("  代币名称")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		TextField {
			id: name
			focus: true
			Layout.fillWidth: true
			Layout.minimumWidth: form.minimumInputSize
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
			placeholderText: form.placeholderText
		}
		
		Label {
			y: 61
			text: qsTr("  描述")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		TextField {
			id: balance
			Layout.fillWidth: true
			Layout.minimumWidth: form.minimumInputSize
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
			placeholderText: form.placeholderText
		}
		
		Label {
			y: 106
			text: qsTr("  总额")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		TextField {
			id: unconfirmed_balance
			Layout.fillWidth: true
			Layout.minimumWidth: form.minimumInputSize
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
			placeholderText: form.placeholderText
		}

		Label {
			y: 151
			text: qsTr("  精度")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		TextField {
			id: total_balance
			Layout.fillWidth: true
			Layout.minimumWidth: form.minimumInputSize
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
			placeholderText: form.placeholderText
		}

		Label {
			y: 196
			text: qsTr("")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		Label {
			id: err_info
			y: 196
			text: qsTr("")
			color: "red"
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}	

		Label {
			y: 241
			text: qsTr("")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		Label {
			y: 241
			text: qsTr("")
			Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
		}

		Button {
			text: qsTr("创建")
			anchors {
				right: parent.right
				bottom: parent.bottom
				margins: 2
			}

			onClicked: {
				err_info.text = qsTr("正在创建，请稍等...");
				err_info.color = "green";
				
				var ret = 0;
				ret = AssetListModal.create_asset(form.name.text, form.balance.text, form.unconfirmed_balance.text, form.total_balance.text);
				if(ret == -1){
					err_info.text = qsTr("请正确填写代币的创建信息！");
					err_info.color = "red";		
				}if(ret == -2){
					err_info.text = qsTr("代币金额设置错误！");
					err_info.color = "red";						
				}if(ret == -3){
					err_info.text = qsTr("代币已存在，请重新设置代币名！");
					err_info.color = "red";						
				}if(ret == -4){
					err_info.text = qsTr("创建代币拥有者失败！");
					err_info.color = "red";						
				}if(ret == -5){
					err_info.text = qsTr("创建代币失败(内部错误)！");
					err_info.color = "red";											
				}if(ret == 0){
					close();
				}
			}
		}		
	}
		
    function createAsset() {
        form.name.clear();
        form.balance.clear();
        form.unconfirmed_balance.clear();
        form.total_balance.clear();
		err_info.text = qsTr("");

        dialog.title = qsTr("创建代币");
        dialog.open();
    }

    function editAsset(asset) {

        dialog.title = qsTr("Edit Asset");
        dialog.open();
    }	
	
	//onAccepted: AssetListModal.create_asset(form.name.text, form.balance.text, form.unconfirmed_balance.text, form.total_balance.text)
}

/*
Dialog {
    id: dialog
	standardButtons: StandardButton.Ok|StandardButton.Cancel

    signal finished(string name, string balance, string unconfirmed_balance, string total_balance)

    function createAsset() {
        form.name.clear();
        form.balance.clear();
        form.unconfirmed_balance.clear();
        form.total_balance.clear();

        dialog.title = qsTr("Add Asset");
        dialog.open();
    }

    function editAsset(asset) {
        form.name.text = asset.name;
        form.balance.text = asset.balance;
        form.unconfirmed_balance.text = asset.unconfirmed_balance;
        form.total_balance.text = asset.total_balance;

        dialog.title = qsTr("Edit Asset");
        dialog.open();
    }

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    //focus: true
    //modal: true
	modality: Qt.ApplicationModal
    title: qsTr("Add Asset")

    AssetForm {
        id: form
        implicitWidth: 400
        implicitHeight: 100		
    }

    onAccepted: AssetListModal.create_asset(form.name.text, form.balance.text, form.unconfirmed_balance.text, form.total_balance.text)
        //finished(form.name.text, form.balance.text, form.unconfirmed_balance.text, form.total_balance.text)
}
*/