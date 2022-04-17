import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick3D 1.15
import QtQuick3D.Helpers 1.15
import QtQuick.Controls.Material 2.0
import "imports/Model"

Page {
    id: mainView

    readonly property real listItemWidth: 220
    readonly property real listItemHeight: 60

    BrainModel {
        id: brainModel
        anchors.fill: parent
        rotationAni: true
    }

    ListModel {
        id: demosModel
        ListElement {//FULL
            name: "全览"
            running: true
            brainstate: "FULL"
            tipshow: "hide"
        }
        ListElement {//XIAQIUNAO
            name: "下丘脑"
            running: false
            brainstate: "XIAQIUNAO"
        }
        ListElement {//SONGGUOTI
            name: "松果体"
            running: false
            brainstate: "SONGGUOTI"
        }
        ListElement {//CHUITI
            name: "垂体"
            running: false
            brainstate: "CHUITI"
        }
        ListElement {//HALFBRAIN
            name: "剖面结构"
            running: false
            brainstate: "HALFBRAIN"
            tipshow: "show"
        }
    }

    Component {
        id: listComponent
        Button {
            width: mainView.listItemWidth
            height: mainView.listItemHeight
            background: Rectangle {
                id: buttonBackground
                border.width: 0.5
                border.color: "#d0808080"
                color: "#d0404040"
                opacity: hovered ? 1.0 : 0.5
            }
            contentItem: Text {
                anchors.centerIn: parent
                color: "#f0f0f0"
                font.pointSize: 20
                text: name
            }

            onClicked: {
                brainModel.rotationAni = running
                brainModel.brainstate = brainstate
                if(tipshow === "show")
                    show.start()
                else
                    hide.start()
            }
        }
    }

    ListView {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20

        id: demosListView
        width: mainView.listItemWidth
        height: count * mainView.listItemHeight
        model: demosModel
        delegate: listComponent
    }

    Item { //胼胝体指示
        id: tip
        x: 1230
        y: 315
        opacity: 0

        Rectangle {
            x: -250
            y: 210
            width: 150
            height: 2
            rotation: -45
        }

        Rectangle {
            x: -123
            y: 157
            width: 100
            height: 2
        }

        Rectangle {
            width: 200
            height: 300
            radius: 10
            opacity: 0.5

            Text {
                text: "胼胝体"
                y: 25
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 30
            }
            Text {
                text: "   状态：良"
                y: 85
                font.pixelSize: 20
            }
            Text {
                text: "   A活跃值：95%"
                y: 120
                font.pixelSize: 20
            }
            Text {
                text: "   B活跃值：90%"
                y: 160
                font.pixelSize: 20
            }
            Text {
                text: "   C活跃值：80%"
                y: 200
                font.pixelSize: 20
            }
        }

        PropertyAnimation {
            id: show
            target: tip
            property: "opacity"
            duration: 1000
            to: 1
        }
        PropertyAnimation {
            id: hide
            target: tip
            property: "opacity"
            duration: 1000
            to: 0
        }
    }
}
