import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import "imports/LottieAnimation"

Window {
    id: root
    width: 1920
    height: 1200
    visible: true
    title: qsTr("BaoHuSan")
    color: "#00000000"
    //visibility: "FullScreen"

    flags: Qt.FramelessWindowHint |
           Qt.WindowMinimizeButtonHint |
           Qt.Window
    //返回键属性
    property string lastpage: "qrc:/login.qml"

    //属性
    property var locale: Qt.locale()
    property date currentDate: new Date()
    property string dateString
    property string timeString
    property string bloodOxpercent
    property string heartpercent
    property string maibopercent

    readonly property color colorGlow: "#1d6d64"
    readonly property color colorWarning: "#d5232f"
    readonly property color colorMain: "#6affcd"
    readonly property color colorBright: "#ffffff"
    readonly property color colorLightGrey: "#888"
    readonly property color colorDarkGrey: "#333"

    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 1
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 2
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 2.5
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 5

    //voiceAssistant
    property int voiceAssistantX //用来存储主窗口x坐标
    property int voiceAssistantY //存储窗口y坐标
    property int xMouse //存储鼠标x坐标
    property int yMouse //存储鼠标y坐标

    Rectangle {
        id: mainRec
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0


        Rectangle {
            id: moveArea
            width: parent.width
            height: 40
            anchors.top: root.bottom
            color: "#00B8A9"

            Text {
                id: title
                text: "保 护 伞"
                anchors.centerIn: parent
                font.pixelSize: 25
                font.bold: true
                color: "white"
            }

            Row {
                id: row
                width: 65
                height: 30
                anchors.right: parent.right
                spacing: 5
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    id: miniButton
                    width: 30
                    height: 30
                    color: "#29CB97"
                    radius: miniButton.width

//                    Image {
//                        id: miniicon
//                        source: "qrc:/images/return.png"
//                        anchors.fill: parent
//                    }

                    MouseArea {
                        id: backArea
                        anchors.fill: parent

                        Connections {
                            target: backArea

                            function onClicked() {
                                //root.visibility = "Minimized"
                                if(lastpage === "qrc:/mask.qml"){
                                    loader.push("qrc:/mask.qml")
                                    lastpage = "qrc:/mainpanel.qml"
                                }
                                else if(lastpage === "qrc:/maskone.qml" || lastpage === "qrc:/masktwo.qml" || lastpage === "qrc:/maskthree.qml" || lastpage === "qrc:/maskfour.qml")
                                    loader.push("qrc:/mask.qml")
                                else if(lastpage === "qrc:/mainpanel.qml"){
                                    loader.push("qrc:/mainpanel.qml")
                                }else {

                                }
                            }
                        }
                    }
                }
                Rectangle {
                    id: closeButton
                    width: 30
                    height: 30
                    color: "#F65860"
                    radius: closeButton.width

//                    Image {
//                        id: closeicon
//                        source: "qrc:/images/close.png"
//                        anchors.fill: parent
//                    }

                    MouseArea {
                        id: closeArea
                        anchors.fill: parent

                        Connections {
                            target: closeArea

                            function onClicked() {
                                Qt.quit()
                            }
                        }
                    }
                }
            }
        }


        Rectangle {
            id: mainArea
            x: 0
            y: 50
            width: root.width
            height: root.height - moveArea.height
            //color: "black"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            StackView {
                id: loader
                anchors.fill: parent
                initialItem: "qrc:/login.qml"
                //initialItem: "qrc:/brain.qml"
            }

            Rectangle { // voice animation Rec
                id: voiceAssistant
                width: voiceAni.width - 100
                height: 50
                x: root.width - 500
                y: root.height - 100
                color: "#00000000"

                LottieAnimation {
                    id: voiceAni
                    anchors.fill: parent
                    source: "qrc:/assets/voice.json"
                    running: true
                    speed: 1
                    loops: Animation.Infinite
                    reverse: false
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        loader.push("qrc:/mainpanel.qml")
                        view.captureTimer.stop()
                    }
                    onPressed: { //接收鼠标按下事件
                        xMouse = mouseX
                        yMouse = mouseY
                        voiceAssistantX = voiceAssistant.x
                        voiceAssistantY = voiceAssistant.y
                    }
                    onPositionChanged: { //鼠标按下后改变位置
                        voiceAssistant.x = voiceAssistantX + (mouseX - xMouse)
                        voiceAssistant.y = voiceAssistantY + (mouseY - yMouse)
                    }
                }

                Component.onCompleted: {
                    voiceFuc.appInit()
                }

                Connections {
                    target: voiceFuc

                    function onInitStatusChanged(initStatus) {
                        if(initStatus === "true") {
                            console.log(initStatus)
                            voiceFuc.baidu_TextToVoice_replyFinish()
                        }
                    }
                }
            }
        }
    }
}
