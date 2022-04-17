import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15
import QtQuick.Layouts 1.15
import "imports/LottieAnimation"
import "imports/CameraView"
import "imports"

Page {
    id: pagelogin

    Rectangle {
        id: loginWindow
        anchors.fill: parent

        Image {
            anchors.fill: parent
            source: "qrc:/images/loginback.jpg"
        }

        Popup {//注册
            id: popup
            width: 400
            height: 500
            anchors.centerIn: parent
            modal: true

            Label {
                text: "正在开发"
                font.bold: true
                font.pixelSize: 30
                anchors.centerIn: parent
            }
            Button {
                id: back
                text: qsTr("返回")
                font.bold: true
                font.pixelSize: 30
                Layout.fillWidth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                onClicked: {
                    popup.close()
                    loader.push("qrc:/login.qml")
                }
            }
        }

        Rectangle { //face Dtection Rec
            id: faceDtecRec
            width: 400
            height: 400
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 250

            color: "#00000000"

            CameraView {//人脸检测组件
                id: view
            }

            LottieAnimation {
                id: faceDtecAni1
                width: faceDtecRec.width - 12
                height: faceDtecRec.height - 60
                anchors.centerIn: faceDtecRec
                source: "qrc:/assets/scan-matrix.json"
                running: true
                speed: 1
                loops: Animation.Infinite
                reverse: false
            }

            LottieAnimation {
                id: faceDtecAni2
                width: faceDtecRec.width
                height: faceDtecRec.height
                anchors.centerIn: faceDtecRec
                source: "qrc:/assets/faceDtecFrame.json"
                running: true
                speed: 1
                loops: Animation.Infinite
                reverse: false
            }
        }

        Rectangle {//注册传送门
            width: 200
            height: 120
            color: "transparent"
            anchors.right: faceDtecRec.left
            anchors.rightMargin: 360
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 190

            //点击事件
            MouseArea {
                anchors.fill: parent
                onClicked: popup.open()
            }
        }
    }
}
