/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick
import QtQuick.Controls
import UntitledProject2

Rectangle {
    width: Constants.width
    height: Constants.height

    color: Constants.backgroundColor

    Rectangle {
        width: Constants.width
        height: Constants.height
        color: Constants.backgroundColor
        ScrollView {
            anchors.fill: parent
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 8
            anchors.bottomMargin: -8
            ListView {
                width: parent.width
                model: 20
                delegate: ItemDelegate {
                    width: parent.width
                    text: "Item " + (index + 1)
                    font.pixelSize: Constants.largeFont.pixelSize
                    font.family: Constants.largeFont.family
                }
            }
        }

        Rectangle {
            id: rectangle
            x: 0
            y: -5
            width: 720
            height: 1293
            color: "#262624"
            Rectangle {
                id: rectangle1
                x: 78
                width: 565
                height: 509
                color: "#30302e"
                radius: 35
                anchors.top: parent.top
                anchors.topMargin: 120
                topRightRadius: 50
                topLeftRadius: 50
                Text {
                    id: text1
                    x: 159
                    width: 247
                    height: 45
                    color: "#ffffff"
                    text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\nhr { height: 1px; border-width: 0; }\nli.unchecked::marker { content: \"\\2610\"; }\nli.checked::marker { content: \"\\2612\"; }\n</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:700;\">Wait Less Lift More</span></p></body></html>"
                    anchors.top: parent.top
                    anchors.topMargin: 31
                    font.pixelSize: 30
                    textFormat: Text.RichText
                    font.family: "Verdana"
                }

                Rectangle {
                    id: rectangle2
                    x: 44
                    width: 477
                    height: 5
                    color: "#383835"
                    radius: 10
                    anchors.top: parent.top
                    anchors.topMargin: 82
                }

                Text {
                    id: text2
                    x: 44
                    width: 463
                    height: 185
                    color: "#ffffff"
                    text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\nhr { height: 1px; border-width: 0; }\nli.unchecked::marker { content: \"\\2610\"; }\nli.checked::marker { content: \"\\2612\"; }\n</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">1. Find Least congested time today</span></p>\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:6pt;\"><br /></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">2. Check congestion for a date/time</span></p>\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:6pt;\"><br /></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">3. Compare congestion across the week</span></p>\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:6pt;\"><br /></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">4. Run Benchmark (Heap vs Hash Map)</span></p>\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:6pt;\"><br /></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">5. Exit</span></p></body></html>"
                    anchors.top: parent.top
                    anchors.topMargin: 102
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignLeft
                    transformOrigin: Item.Center
                    textFormat: Text.RichText
                    Rectangle {
                        id: rectangle3
                        x: 8
                        y: 271
                        width: 459
                        height: 86
                        color: "#383835"
                        radius: 20
                        Rectangle {
                            id: rectangle4
                            x: 3
                            width: 453
                            height: 80
                            color: "#30302e"
                            radius: 20
                            anchors.top: parent.top
                            anchors.topMargin: 3
                            scale: 1
                            TextInput {
                                id: textInput
                                x: 8
                                width: 437
                                height: 64
                                text: "Enter Choice"
                                anchors.top: parent.top
                                anchors.topMargin: 8
                                font.pixelSize: 25
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                selectedTextColor: "#000000"
                                font.weight: Font.Medium
                                font.family: "Verdana"
                                font.bold: false
                                cursorVisible: true
                            }
                        }
                    }
                }
                bottomRightRadius: 50
                bottomLeftRadius: 50
            }
        }
    }
}
