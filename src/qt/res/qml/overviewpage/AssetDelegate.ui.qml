

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
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.1

ItemDelegate {
    id: delegate
    checkable: true

    Rectangle {
        id: background
        //anchors.TopAnchor: parent
        width: contentItem.width * 2
        height: label1.height * 2
        color: Qt.rgba(215 / 255, 215 / 255, 215 / 255, 1) //(1)
        opacity: 1 //(2)
        Text {
            id: myname
            y: 4
            x: 8
            text: name
            font.bold: true
            font.pixelSize: 15
            elide: Text.ElideRight
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        
        Image {
            id: lockIcon
            y: 0
            x: parent.width / 2 - 5
            height: 24
            width: 24
            smooth: true
            source: locked? "qrc:/icons/lock_closed" : ""
            visible: IsMainWallet ? false : true
        }

        QQC1.Switch {
            id: lockAsset
            visible: IsMainWallet ? true : false
            y: 2
            x: parent.width / 2 - 25
            height: 20
            width: 42
            checked: locked? false : true
            enabled: AssetNo == 0? false : true
            style: SwitchStyle {
                groove: Rectangle {
                    implicitWidth: 42
                    implicitHeight: 20
                    radius: 4
                    border.color: control.activeFocus ? "darkblue" : "gray"
                    border.width: 1
                    color: lockAsset.checked? "green" : "gray"
                }
            }

            MessageDialog {
                id: lockasset_msgbox
                title: !lockAsset.checked ? "锁定代币" : "解锁代币"
                modality: Qt.ApplicationModal
                icon: StandardIcon.Question
                text: ""
                informativeText: !lockAsset.checked ? "确定要锁定吗?" : "确定要解锁吗?"
                standardButtons: StandardButton.Yes | StandardButton.No
                onYes: { AssetListModal.lock_asset(AssetNo, !lockAsset.checked) }
                onNo: { lockAsset.checked = !lockAsset.checked }
            }

            onClicked:{
                if(!lockAsset.checked){
                    lockasset_msgbox.text = "将要锁定代币" + name + ", 锁定后用户将不能继续使用"
                }else{
                    lockasset_msgbox.text = "将要解锁代币" + name + ", 解锁后用户才能继续正常使用"
                }
                lockasset_msgbox.visible = true
            }
        }
    }


    contentItem: ColumnLayout {
        spacing: 10

        GridLayout {
            id: grid
            visible: true

            Layout.topMargin: 18
            columns: 2
            rowSpacing: 10
            columnSpacing: 10

            Label {
                id: label1
                text: qsTr("金额:")
                Layout.leftMargin: 10
            }

            Label {
                id: label4
                text: balance
                font.bold: false
                elide: Text.ElideRight
                Layout.fillWidth: false
                width: 185
            }

            Label {
                id: label2
                text: qsTr("未确认金额:")
                Layout.leftMargin: 10
            }

            Label {
                id: label5
                text: unconfirmed_balance
                font.bold: false
                elide: Text.ElideRight
                Layout.fillWidth: true
                width: 185
            }

            Label {
                id: label3
                text: qsTr("可用金额:")
                Layout.leftMargin: 10
            }

            Label {
                id: label6
                text: total_balance
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
                width: 185
            }
        }
    }

    states: [
        State {
            name: "expanded"
            when: delegate.checked

            PropertyChanges {
                target: grid
                x: 1
                y: 20
                width: 316
                transformOrigin: Item.Center
                Layout.columnSpan: 1
                columns: 2
                columnSpacing: 8
                visible: true
            }

            PropertyChanges {
                target: label
                width: 171
                topPadding: -2
                styleColor: "#ef1010"
                font.bold: true
                visible: true
            }

            PropertyChanges {
                target: label1
                x: 8
                horizontalAlignment: Text.AlignLeft
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.fillWidth: false
            }

            PropertyChanges {
                target: label2
                x: 8
            }

            PropertyChanges {
                target: label3
                x: 8
            }

            PropertyChanges {
                target: label4
                x: 146
                width: 165
            }

            PropertyChanges {
                target: label5
                x: 146
                width: 165
            }

            PropertyChanges {
                target: label6
                x: 146
                width: 165
            }
        }
    ]
}
