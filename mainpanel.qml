import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.15
import "imports/LottieAnimation"

Page {
    Rectangle {
        id: mainRec
        anchors.fill: parent
        color: "transparent"

//        ParticleSystem {
//            id: particles
//            anchors.fill: parent

//            ImageParticle {
//                groups: ["center","edge"]
//                anchors.fill: parent
//                source: "qrc:/images/glowdot.png"
//                colorVariation: 0.1
//                color: "#009999FF"
//            }

//            Emitter {
//                anchors.fill: parent
//                group: "center"
//                emitRate: 400
//                lifeSpan: 2000
//                size: 20
//                sizeVariation: 2
//                endSize: 0
////                //! [0]
////                shape: EllipseShape {fill: true}
//                velocity: TargetDirection {
//                    targetX: root.width/2
//                    targetY: root.height/2
//                    proportionalMagnitude: true
//                    magnitude: 0.5
//                }
////                //! [0]
//            }

//            Emitter {
//                anchors.fill: parent
//                group: "edge"
//                startTime: 2000
//                emitRate: 2000
//                lifeSpan: 2000
//                size: 28
//                sizeVariation: 2
//                endSize: 16
//                //shape: EllipseShape {fill: false}
//                velocity: TargetDirection {
//                    targetX: root.width/2
//                    targetY: root.height/2
//                    proportionalMagnitude: true
//                    magnitude: 0.1
//                    magnitudeVariation: 0.1
//                }
//                acceleration: TargetDirection {
//                    targetX: root.width/2
//                    targetY: root.height/2
//                    targetVariation: 200
//                    proportionalMagnitude: true
//                    magnitude: 0.1
//                    magnitudeVariation: 0.1
//                }
//            }
//        }

        Rectangle {
            id:coverflow
            width: 1200
            height: 600
            color: "transparent"
            anchors.horizontalCenter: mainRec.horizontalCenter
            y: mainRec.height / 2 - height / 2 + 50

            property int itemCount: 3

            ListModel {
                id: model
                ListElement{
                    url: "qrc:/assets/mask.json"
                    title: "训练中心"
                    pageurl: "qrc:/mask.qml"
                    mlastpage: "qrc:/mainpanel.qml"
                }
                ListElement{
                    url: "qrc:/assets/monitor.json"
                    title: "监测中心"
                    pageurl: "qrc:/monitor.qml"
                    mlastpage: "qrc:/mainpanel.qml"
                }
                ListElement{
                    url: "qrc:/assets/advice.json"
                    title: "健康评估"
                    pageurl: "qrc:/advice.qml"
                    mlastpage: "qrc:/mainpanel.qml"
                }
            }

            PathView {
                id: pathView
                model: model
                anchors.fill: parent

                delegate: Item {
                    id: delegateItem
                    width: 600
                    height: 700
                    z:PathView.iconZ
                    scale:PathView.iconScale

                    Rectangle {
                        id: page
                        width: delegateItem.width
                        height: delegateItem.height
                        radius: 20
                        //color: "#DBDBD3"
                        //color: "transparent"

                        LottieAnimation {
                            id: voiceAni
                            anchors.fill: parent
                            source: url
                            running: true
                            speed: 1
                            loops: Animation.Infinite
                            reverse: false
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                loader.push(pageurl)
                                root.lastpage = mlastpage
                            }
                        }

                        Behavior on width { SmoothedAnimation { velocity: 200 }}
                        Behavior on height { SmoothedAnimation { velocity: 200 }}
                        Behavior on radius { SmoothedAnimation { velocity: 200 }}
                    }

                    Label {
                        id: text
                        text: title
                        font.pixelSize: 60
                        font.bold: true
                        anchors.top: page.bottom
                        anchors.topMargin: 20
                        anchors.horizontalCenter: page.horizontalCenter
                        color: "white"
                    }

                    ShaderEffect {
                        anchors.top: page.bottom
                        width: page.width
                        height: page.height;
                        anchors.left: page.left
                        property variant source: page;
                        property size sourceSize: Qt.size(0.5 / page.width, 0.5 / page.height);
                        fragmentShader:
                            "varying highp vec2 qt_TexCoord0;
                                uniform lowp sampler2D source;
                                uniform lowp vec2 sourceSize;
                                uniform lowp float qt_Opacity;
                                void main() {

                                    lowp vec2 tc = qt_TexCoord0 * vec2(1, -1) + vec2(0, 1);
                                    lowp vec4 col = 0.25 * (texture2D(source, tc + sourceSize) + texture2D(source, tc- sourceSize)
                                    + texture2D(source, tc + sourceSize * vec2(1, -1))
                                    + texture2D(source, tc + sourceSize * vec2(-1, 1)));
                                    gl_FragColor = col * qt_Opacity * (1.0 - qt_TexCoord0.y) * 0.2;
                                }"
                    }

                    transform: Rotation{
                        origin.x:page.width/2.0
                        origin.y:page.height/2.0
                        axis{x:0;y:1;z:0}
                        angle: delegateItem.PathView.iconAngle
                    }
                }
                path:coverFlowPath
                pathItemCount: coverflow.itemCount

                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5

            }

            Path {
                id:coverFlowPath
                startX: 0
                startY: coverflow.height/3

                PathAttribute{name:"iconZ";value: 0}
                PathAttribute{name:"iconAngle";value: 0}
                PathAttribute{name:"iconScale";value: 0.6}

                PathLine{x:coverflow.width/2;y:coverflow.height/3}

                PathAttribute{name:"iconZ";value: 10}
                PathAttribute{name:"iconAngle";value: 0}
                PathAttribute{name:"iconScale";value: 1.0}

                PathLine{x:coverflow.width;y:coverflow.height/3}

                PathAttribute{name:"iconZ";value: 0}
                PathAttribute{name:"iconAngle";value: 0}
                PathAttribute{name:"iconScale";value: 0.6}
                PathPercent{value:1.0}
            }
        }
    }
}
