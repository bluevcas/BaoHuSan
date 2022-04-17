import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0
import QtCharts 2.15
import "imports/LottieAnimation"
import "imports/Model"


Page {
    Rectangle {
        id: rootRec
        anchors.fill: parent
        color: "#00B8A9"

        Frame {
            id: mainFrame
            anchors.fill: parent
            anchors.margins: 10

            Timer { //数据更新计时器
                id: timer
                interval: 1000
                running: true
                repeat: true
                onTriggered:{
                    dateString = (new Date()).toLocaleDateString()
                    timeString = (new Date()).toLocaleTimeString()
                    bloodOx.text = 80 + Math.round((Math.random() * 10 + 1)) + " %"
                    heart.text = (85 + Math.round((Math.random() * 10 + 1))) + " - " + (80 + Math.round((Math.random() * 10 + 1)))
                }
            }

            RowLayout {
                id: mainRowLayout
                anchors.fill: parent
                anchors.margins: 24
                spacing: 36

                ColumnLayout {
                    id: leftCol
                    spacing: 16
                    Layout.preferredWidth: 200
                    Layout.fillHeight: true

                    Label {
                        text: qsTr("心率 - 脉搏")
                        color: "white"
                        font.pixelSize: fontSizeMedium
                    }
                    //心率监测

                    Frame {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.minimumWidth: 150
                        //Layout.minimumHeight: 64
                        Layout.preferredWidth: 300
                        //Layout.preferredHeight: 128
                        Layout.maximumWidth: 300
                        //Layout.maximumHeight: 256
                        Layout.fillHeight: true

                        LottieAnimation {
                            id: faceDtec
                            height: 200
                            anchors.fill: parent
                            source: "qrc:/assets/heart.json"
                            running: true
                            speed: 1
                            loops: Animation.Infinite
                            reverse: false

                            Label {
                                id: heart
                                text: "98" + " - " + "80"
                                color: "white"
                                font.pixelSize: Qt.application.font.pixelSize * 2
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 20
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }

                    RowLayout {
                        Layout.topMargin: 16

                        Label {
                            id: bloodOption
                            text: qsTr("血氧")
                            color: "white"
                            font.pixelSize: fontSizeMedium
                            horizontalAlignment: Label.AlignLeft

                            Layout.fillWidth: true
                        }
                    }

                    Frame {
                        id: pressureFrame
                        leftPadding: 1
                        rightPadding: 1
                        topPadding: 1
                        bottomPadding: 1

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight: 128

                        //血氧
                        LottieAnimation {
                            id: pressure
                            height: parent.height
                            anchors.centerIn: parent
                            source: "qrc:/assets/pressure.json"
                            running: true
                            speed: 1
                            loops: Animation.Infinite
                            fillMode:Image.PreserveAspectFit
                            reverse: false

                            Label {
                                id: bloodOx
                                text: "85 %"
                                color: "white"
                                font.pixelSize: Qt.application.font.pixelSize * 2
                                anchors.centerIn: parent
                            }
                        }
                    }
                }

                Rectangle {
                    color: colorMain
                    implicitWidth: 1
                    Layout.fillHeight: true
                }

                ColumnLayout { //Middle
                    //Layout.preferredWidth: parent.width - leftCol.width - rightCol.width - 200
                    Layout.preferredWidth: 700
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Label {
                        id: timeLabel
                        text: qsTr(timeString)
                        font.pixelSize: fontSizeExtraLarge

                        Layout.alignment: Qt.AlignHCenter
                    }

                    Label {
                        //text: qsTr("01/01/2022")
                        text: qsTr(dateString)
                        color: colorLightGrey
                        font.pixelSize: fontSizeMedium

                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 2
                        Layout.bottomMargin: 10
                    }

                    Rectangle {
                        id: modelRec
                        color: "transparent"
                        Layout.preferredWidth: 300
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        //3D Model
                        Model {
                            id: model
                            anchors.centerIn: modelRec
                            //eulerRotationy: movearea.x

                            MouseArea {
                                anchors.fill: parent
                                onClicked: loader.push("qrc:/brain.qml")
                            }
                        }
                        //                        MouseArea {
                        //                            id: movearea
                        //                            anchors.fill: parent
                        //                            drag.target: movearea
                        //                        }


                    }
                }

                Rectangle {
                    color: colorMain
                    implicitWidth: 1
                    Layout.fillHeight: true
                }

                ColumnLayout {
                    id: rightCol
                    Row {
                        spacing: 8

                        Image {
                            source: "qrc:/images/weather.png"
                        }

                        Column {
                            spacing: 8

                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter

                                Label {
                                    id: outsideTempValueLabel
                                    text: qsTr("11")
                                    font.pixelSize: fontSizeExtraLarge
                                }

                                Label {
                                    text: qsTr("°C")
                                    font.pixelSize: Qt.application.font.pixelSize * 2.5
                                    anchors.baseline: outsideTempValueLabel.baseline
                                }
                            }

                            Label {
                                text: qsTr("上海, 中国")
                                color: colorLightGrey
                                font.pixelSize: fontSizeMedium
                            }
                        }
                    }

                    ColumnLayout {
                        id: playRowLayout
                        spacing: 8

                        Layout.preferredWidth: 160
                        Layout.preferredHeight: 380
                        Layout.fillHeight: true

                        Item {
                            Layout.fillHeight: true
                        }

                        //                        Label {
                        //                            text: qsTr("能力曲线")
                        //                            color: "white"
                        //                            font.pixelSize: fontSizeMedium
                        //                        }

                        Frame {
                            id: playFrame
                            Layout.alignment: Qt.AlignHCenter
                            Layout.minimumWidth: 200
                            Layout.preferredWidth: 360
                            Layout.maximumWidth: 360
                            Layout.fillHeight: true

                            //能力曲线
                            Rectangle {
                                id: playRec
                                anchors.fill: parent

                                SwipeView {
                                    id: view

                                    currentIndex: 1
                                    anchors.fill: parent
                                    clip: true

                                    Image {
                                        id: firstPage
                                        fillMode: Image.Stretch
                                        source: "qrc:/images/abilityline.png"
                                    }
                                    Image {
                                        id: secondPage
                                        fillMode: Image.Stretch
                                        source: "qrc:/images/abilitypie.png"
                                    }
                                    Image {
                                        id: thirdPage
                                        fillMode: Image.Stretch
                                        source: "qrc:/images/abilitywarning.png"
                                    }
                                }

                                PageIndicator {
                                    id: indicator

                                    count: view.count
                                    currentIndex: view.currentIndex

                                    anchors.bottom: view.bottom
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                            }
                        }
                    }

                    //日训练进度
                    Item {
                        implicitHeight: dayPlan.implicitHeight
                        Layout.fillWidth: true
                        Layout.topMargin: 16

                        Label {
                            text: qsTr("日训练进度")
                            anchors.baseline: dayPlan.bottom
                            anchors.left: parent.left
                        }

                        Label {
                            id: dayPlan
                            text: qsTr("70 %")
                            font.pixelSize: fontSizeLarge
                            anchors.right: parent.right
                        }
                    }

                    Slider {
                        value: 0.35
                        Layout.fillWidth: true
                    }

                    //周训练进度
                    Item {
                        implicitHeight: weekPlan.implicitHeight
                        Layout.fillWidth: true
                        Layout.topMargin: 16

                        Label {
                            text: qsTr("周训练进度")
                            anchors.baseline: weekPlan.bottom
                            anchors.left: parent.left
                        }

                        Label {
                            id: weekPlan
                            text: qsTr("30 %")
                            font.pixelSize: fontSizeLarge
                            anchors.right: parent.right
                        }
                    }

                    Slider {
                        value: 0.25
                        Layout.fillWidth: true
                    }

                    //月训练进度
                    Item {
                        implicitHeight: monthPlan.implicitHeight
                        Layout.fillWidth: true
                        Layout.topMargin: 16

                        Label {
                            text: qsTr("月训练进度")
                            anchors.baseline: monthPlan.bottom
                            anchors.left: parent.left
                        }

                        Label {
                            id: monthPlan
                            text: qsTr("10 %")
                            font.pixelSize: fontSizeLarge
                            anchors.right: parent.right
                        }
                    }

                    Slider {
                        value: 0.25
                        Layout.fillWidth: true
                    }

                    //                        Item {
                    //                            Layout.fillHeight: true
                    //                        }
                }
            }
        }

        SequentialAnimation {
            running: true
            ParallelAnimation {
                NumberAnimation {
                    target: mainFrame
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 1500
                }
            }
        }
    }
}
