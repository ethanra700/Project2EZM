pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Project2

Rectangle {
    id: root
    width: Constants.width
    height: Constants.height
    color: "#242522"
    Rectangle {
        id: card
        width: Math.min(root.width - 64, 572)
        height: 508
        anchors.horizontalCenter: parent.horizontalCenter
        y: 132
        radius: 48
        color: "#30312e"
        Column {
            id: menu
            anchors.top: parent.top
            anchors.topMargin: 32
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 44
            anchors.rightMargin: 44
            spacing: 0

            Text {
                width: parent.width
                text: "Wait Less Lift More"
                color: "#f1f1ef"
                font.family: "Arial"
                font.pixelSize: 17
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Item { width: 1; height: 31 }

            Rectangle {
                width: parent.width
                height: 4
                radius: 2
                color: "#383936"
            }

            Item { width: 1; height: 15 }

            Repeater {
                model: ["1. Find least congested time today", "2. Check congestion for a date and time", "3. Compare congestion across the week", "4. Run benchmark (Heap vs Hash Map)"]
                delegate: Text {
                    required property string modelData
                    width: parent.width
                    height: 29
                    text: modelData
                    color: "#dededb"
                    font.family: "Arial"
                    font.pixelSize: 14
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item { width: 1; height: 118 }

            Item {
                width: parent.width
                height: 37

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Enter Choice"
                    color: "#d0d1cd"
                    font.family: "Arial"
                    font.pixelSize: 20
                }
            }

            Item {
                width: parent.width
                height: 86

                TextField {
                    id: choice
                    width: parent.width - 16
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    placeholderText: ""
                    color: "#f0f0ed"
                    font.family: "Arial"
                    font.pixelSize: 26
                    leftPadding: 16
                    rightPadding: 16
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: TextInput.AlignVCenter
                    selectByMouse: true
                    focus: true
                    background: Rectangle {
                        radius: 21
                        color: "#2f302d"
                        border.width: 2
                        border.color: "#3b3c39"
                    }

                    onAccepted: root.enter()
                }
            }

        }
    }
}
