import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick3D 1.15
import "imports/Capsule"
import "imports/FireMan"

Page {//胶囊大战
    Rectangle {
        id: mainRec
        anchors.fill: parent
        color: "transparent"

        Image {
            source: "qrc:/images/sky.png"
            anchors.fill: parent
        }

        View3D {
            id: view3D
            width: mainRec.width
            height: mainRec.height
            environment: sceneEnvironment

            SceneEnvironment {
                id: sceneEnvironment
                antialiasingQuality: SceneEnvironment.High
                antialiasingMode: SceneEnvironment.MSAA
            }

//            Node {
//                position: Qt.vector3d(0, 0, 0);

//                PerspectiveCamera {
//                    position: Qt.vector3d(0, 0, 600)
//                }

//                eulerRotation.y: -90

//                SequentialAnimation on eulerRotation.y {
//                    loops: Animation.Infinite

//                    PropertyAnimation {
//                        duration: 5000
//                        to: 360
//                        from: 0
//                    }
//                }
//            }

            Node {
                id: scene
                DirectionalLight {
                    id: directionalLight
                }

                PerspectiveCamera {
                    id: camera
                    z: 400
                }

                FireMan {
                    scale: Qt.vector3d(10, 10, 10)
                    eulerRotation.y: mouseArea.x
                    y: -200
                }

                Node {
                    id: shapeSpawner
                    property real range: 100
                    property var instances: []
                    readonly property int maxInstances: 100

                    function addOrRemove(add) {
                        //! [spawner node]
                        if (add) {
                            //! [adding]
                            // Create a new weirdShape at random postion
                            var xPos = (2 * Math.random() * range) - range;
                            var yPos = (2 * Math.random() * range) - range;
                            var zPos = (2 * Math.random() * range) - range;
                            var ShapeComponent = Qt.createComponent("qrc:/imports/Capsule/Capsule.qml");
                            let instance = ShapeComponent.createObject(
                                    shapeSpawner, { "x": xPos, "y": yPos, "z": 0,
                                        "scale": Qt.vector3d(1, 1, 1), "color": "blue"});
                            instances.push(instance);
                            //! [adding]
                        } else {
                            //! [removing]
                            // Remove last item in instances list
                            let instance = instances.pop();
                            instance.destroy();
                            //! [removing]
                        }
                        //countLabel.text = "Models in Scene: " + instances.length;
                    }
                }

                Component.onCompleted: {
                    // Create 10 instances to get started
                    for (var i = 0; i < 3; ++i)
                        shapeSpawner.addOrRemove(true);
                }

                Capsule {
                    eulerRotation.y: mouseArea.x
                    color: "green"
                    x: -250
                    y: -200
                    z: 0

                    PropertyAnimation on eulerRotation.y {
                        from: 0
                        to: 360
                        duration: 3000
                        running: true
                        loops: Animation.Infinite
                    }
                }
                Capsule {
                    eulerRotation.y: mouseArea.x
                    color: "blue"
                    x: -100
                    y: -200
                    z: 0

                    PropertyAnimation on eulerRotation.y {
                        from: 0
                        to: 360
                        duration: 3000
                        running: true
                        loops: Animation.Infinite
                    }
                }
                Capsule {
                    eulerRotation.y: mouseArea.x
                    color: "yellow"
                    x: 250
                    y: -200
                    z: 0

                    PropertyAnimation on eulerRotation.y {
                        from: 0
                        to: 360
                        duration: 3000
                        running: true
                        loops: Animation.Infinite
                    }
                }
                Capsule {
                    eulerRotation.y: mouseArea.x
                    color: "red"
                    x: 100
                    y: -200
                    z: 0

                    PropertyAnimation on eulerRotation.y {
                        from: 0
                        to: 360
                        duration: 3000
                        running: true
                        loops: Animation.Infinite
                    }
                }
            }
        }



        MouseArea {
            id: mouseArea
            width: mainRec.width
            height: mainRec.height
            drag.target: mouseArea
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4}D{i:3}D{i:6}D{i:7}D{i:9}D{i:8}D{i:5}
D{i:2}D{i:10}D{i:1}
}
##^##*/
