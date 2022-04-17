import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import com.app.cvframe 1.0

Page {
    id: pageadvice
    Component.onCompleted: {
        cvThread.runCapture()
    }

//    Connections {
//        target: cvThread

//        function onUpdateView(frame) {
//            selectedImage.setFrame(frame)
//        }
//    }

    Connections {
        target: liveImageProvider

        function onImageChanged()
        {
            opencvImage.reload()
        }
    }

    Rectangle {
        id: bar
        width: parent.width
        height: parent.height * 0.1
        color: "#00A7C0"

        Label {
            text: "行为监测"
            color: "white"
            font.pixelSize: 30
            anchors.centerIn: parent
        }
    }

//    CVFrame {
//        id: selectedImage
//        //            Layout.preferredWidth: imageContainer.width
//        //            Layout.preferredHeight: imageContainer.height
//        anchors.centerIn: parent
//        visible: true
//    }

    Image {
        id: opencvImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        property bool counter: false
        visible: false
        source: "image://live/image"
        asynchronous: false
        cache: false

        function reload()
        {
            counter = !counter
            source = "image://live/image?id=" + counter
        }
    }
}
