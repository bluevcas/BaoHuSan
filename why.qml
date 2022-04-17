import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

Page {//十万个为什么
    Rectangle {
        id: mainRec
        anchors.fill: parent

        Video {
            id: video
            anchors.fill: parent
            //source: "http://www.nbub.com/baohusan/videos/sea.mp4"
            source: "file:///D:/BaoHuSan/PC/images/sea.mp4"
            anchors.centerIn: parent
            //autoPlay: true
            fillMode: Image.Stretch

            focus: true
            Keys.onSpacePressed: video.playbackState == MediaPlayer.PlayingState ? video.pause() : video.play()
            Keys.onLeftPressed: video.seek(video.position - 5000)
            Keys.onRightPressed: video.seek(video.position + 5000)

            onStopped: question.visible = true

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    //(video.stopped || video.paused)  ? video.play() : video.pause()
                    video.playbackState === MediaPlayer.PlayingState ? video.pause() : video.play()
                    question.visible = false
                    tip.visible = false
                }

                onPressed: {
                    question.color = "white"
                    question.visible = true
                }
            }
        }

        Label {
            id: question
            anchors.centerIn: parent
            text: qsTr("问题：画面中共出现了几只小船？")
            font.bold: true
            font.pixelSize: 50
        }

        Label {
            id: tip
            anchors.top: question.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: question.horizontalCenter
            text: qsTr("点击开始播放")
            font.bold: true
            font.pixelSize: 30
        }

        Popup {//答题结果显示
            id: popup
            width: 400
            height: 500
            anchors.centerIn: parent
            modal: true

            Label {
                text: "恭喜你答对了！"
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
                    loader.push("qrc:/mainpanel.qml")
                }
            }
        }

        RowLayout {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 200

            Button {
                id: answerA
                text: qsTr("A: 3只")
                font.bold: true
                font.pixelSize: 70
                Layout.fillWidth: true
            }
            Button {
                id: answerB
                text: qsTr("B: 2只")
                font.bold: true
                font.pixelSize: 70
                Layout.fillWidth: true
                onClicked: popup.open()
            }
            Button {
                id: answerC
                text: qsTr("C: 1只")
                font.bold: true
                font.pixelSize: 70
                Layout.fillWidth: true
            }
        }
    }
}
