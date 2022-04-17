import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {//家人连连看
    Rectangle {
        id: mainRec
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: -76
        anchors.leftMargin: 0
        anchors.topMargin: 0
        color: "#00BCD4"

        RowLayout {
            id: mainRowLayout
            anchors.fill: parent
            anchors.margins: 24
            spacing: 36

            ColumnLayout {
                id: leftCol
                spacing: 16
                Layout.preferredWidth: name1.width
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter

                Label {
                    id: name1
                    text: qsTr("儿子")
                    font.bold: true
                    font.pixelSize: 25
                }

                Label {
                    text: qsTr("女儿")
                    font.bold: true
                    font.pixelSize: 25
                }

                Label {
                    text: qsTr("老伴")
                    font.bold: true
                    font.pixelSize: 25
                }

                Label {
                    text: qsTr("孙子")
                    font.bold: true
                    font.pixelSize: 25
                }

                Label {
                    text: qsTr("孙女")
                    font.bold: true
                    font.pixelSize: 25
                }
            }

            ColumnLayout {
                id: middleCol
                Layout.preferredWidth: dropZone1.width
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter

                DropArea {
                    id: dropZone1
                    width: 300
                    height: 160
                    Drag.keys: [root.thekey1]
                    onDropped: {
                        console.log("dropped!")
                        goal1.color = goalOn
                    }

                    Rectangle {
                        id: goal1
                        anchors.fill: parent
                        color: goalOff
                        border.color: "black"
                        border.width: 5
                    }
                }

                DropArea {
                    id: dropZone2
                    width: 300
                    height: 160
                    Drag.keys: [root.thekey2]
                    onDropped: {
                        console.log("dropped!")
                        goal2.color = goalOn
                    }

                    Rectangle {
                        id: goal2
                        anchors.fill: parent
                        color: goalOff
                        border.color: "black"
                        border.width: 5
                    }
                }

                DropArea {
                    id: dropZone3
                    width: 300
                    height: 160
                    Drag.keys: [root.thekey3]
                    onDropped: {
                        console.log("dropped!")
                        goal3.color = goalOn
                    }

                    Rectangle {
                        id: goal3
                        anchors.fill: parent
                        color: goalOff
                        border.color: "black"
                        border.width: 5
                    }
                }
            }

            ColumnLayout {
                id: rightCol
                Layout.preferredWidth: ball1.width
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter

                Rectangle {
                    id: ball1
                    width: 100
                    height: 100
                    radius: width
                    color: ballOff
                    border.color: "black"
                    border.width: 5

                    Drag.active: dragArea1.drag.active
                    Drag.keys: [root.thekey1]

                    Text {
                        id: title
                        anchors.centerIn: parent
                        text: Math.round(parent.x) + " x " + Math.round(parent.y)
                    }

                    MouseArea {
                        id: dragArea1
                        anchors.fill: ball1
                        drag.target: ball1
                        onPressed: {ball1.color = ballOn; goal1.color = goalOff}
                        onReleased: {ball1.color = ballOff; ball1.Drag.drop()}
                    }
                }

                Rectangle {
                    id: ball2
                    width: 100
                    height: 100
                    radius: width
                    color: ballOff
                    border.color: "black"
                    border.width: 5

                    Drag.active: dragArea2.drag.active
                    Drag.keys: [root.thekey2]

                    Text {
                        id: title2
                        anchors.centerIn: parent
                        text: Math.round(parent.x) + " x " + Math.round(parent.y)
                    }

                    MouseArea {
                        id: dragArea2
                        anchors.fill: ball2
                        drag.target: ball2
                        onPressed: {ball2.color = ballOn; goal2.color = goalOff}
                        onReleased: {ball2.color = ballOff; ball2.Drag.drop()}
                    }
                }

                Rectangle {
                    id: ball3
                    width: 100
                    height: 100
                    radius: width
                    color: ballOff
                    border.color: "black"
                    border.width: 5

                    Drag.active: dragArea3.drag.active
                    Drag.keys: [root.thekey3]

                    Text {
                        id: title3
                        anchors.centerIn: parent
                        text: Math.round(parent.x) + " x " + Math.round(parent.y)
                    }

                    MouseArea {
                        id: dragArea3
                        anchors.fill: ball3
                        drag.target: ball3
                        onPressed: {ball3.color = ballOn; goal3.color = goalOff}
                        onReleased: {ball3.color = ballOff; ball3.Drag.drop()}
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.1;height:480;width:640}D{i:4}D{i:5}D{i:6}D{i:7}
D{i:8}D{i:3}D{i:11}D{i:10}D{i:13}D{i:12}D{i:15}D{i:14}D{i:9}D{i:18}D{i:19}D{i:17}
D{i:21}D{i:22}D{i:20}D{i:24}D{i:25}D{i:23}D{i:16}D{i:2}D{i:1}
}
##^##*/
