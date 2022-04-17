import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0
import "imports"

Page {
    id: pagemask
//    Rectangle {
//        id: mainRec
//        anchors.fill: parent
//        color: "#00BCD4"

//        RowLayout {
//            anchors.fill: parent

//            //侧边栏
//            ControlNav {
//                Layout.alignment: Qt.AlignVCenter
//            }
//        }
//    }

    Image {
        anchors.fill: parent
        source: "qrc:/images/back.jpg"
    }

    Grid {
        width: card1.width * 4 + 90
        height: 300
        anchors.centerIn: parent
        anchors.verticalCenter: parent.verticalCenter

        spacing: 30

        MaskCard {//我的世界
            id: card1
            onClicked: {
                card2.visible = false
                card3.visible = false
                card4.visible = false
                transanim1.start()
            }

            SequentialAnimation {
                id: transanim1
                running: false

                NumberAnimation {
                    target: card1
                    property: "x"
                    to: 1920 / 2 - 460
                    duration: 500
                    easing.type: Easing.OutQuad
                }
                ParallelAnimation {
                    NumberAnimation {
                        target: card1
                        property: "rotation"
                        from: 0
                        to: 360
                        duration: 600
                    }

                    ScaleAnimator {
                        target: card1
                        from: 1
                        to: 0
                        duration: 600
                    }
                }

                onStopped: loader.push("qrc:/createWorld.qml")
            }
            source: "qrc:/Masks/MaskCardsPic/card1.jpg"
        }
        MaskCard {//十万个为什么
            id: card2
            onClicked: {
                card1.visible = false
                card3.visible = false
                card4.visible = false
                transanim2.start()
            }

            SequentialAnimation {
                id: transanim2
                running: false

                NumberAnimation {
                    target: card2
                    property: "x"
                    to: 1920 / 2 - 460
                    duration: 500
                    easing.type: Easing.OutQuad
                }
                ParallelAnimation {
                    NumberAnimation {
                        target: card2
                        property: "rotation"
                        from: 0
                        to: 360
                        duration: 600
                    }
                    ScaleAnimator {
                        target: card2
                        from: 1
                        to: 0
                        duration: 600
                    }
                }

                onStopped: loader.push("qrc:/why.qml")
            }
            source: "qrc:/Masks/MaskCardsPic/card2.jpg"
        }
        MaskCard {//小熊降落
            id: card3
            onClicked: {
                card1.visible = false
                card2.visible = false
                card4.visible = false
                transanim3.start()
            }

            SequentialAnimation {
                id: transanim3
                running: false

                NumberAnimation {
                    target: card3
                    property: "x"
                    to: 1920 / 2 - 460
                    duration: 500
                    easing.type: Easing.OutQuad
                }
                ParallelAnimation {
                    NumberAnimation {
                        target: card3
                        property: "rotation"
                        from: 0
                        to: 360
                        duration: 600
                    }
                    ScaleAnimator {
                        target: card3
                        from: 1
                        to: 0
                        duration: 600
                    }
                }

                onStopped: loader.push("qrc:/bearWhack.qml")
            }
            source: "qrc:/Masks/MaskCardsPic/card3.jpg"
        }
        MaskCard {//翻卡片
            id: card4
            onClicked: {
                card1.visible = false
                card2.visible = false
                card3.visible = false
                transanim4.start()
            }

            SequentialAnimation {
                id: transanim4
                running: false

                NumberAnimation {
                    target: card4
                    property: "x"
                    to: 1920 / 2 - 460
                    duration: 500
                    easing.type: Easing.OutQuad
                }
                ParallelAnimation {
                    NumberAnimation {
                        target: card4
                        property: "rotation"
                        from: 0
                        to: 360
                        duration: 600
                    }
                    ScaleAnimator {
                        target: card4
                        from: 1
                        to: 0
                        duration: 600
                    }
                }

                onStopped: loader.push("qrc:/turnCards.qml")
            }
            source: "qrc:/Masks/MaskCardsPic/card4.jpg"
        }
    }
}
