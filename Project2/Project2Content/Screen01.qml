pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Project2

Rectangle {
    id: root
    width: Constants.width
    height: Constants.height
    color: "#001540"
    property string step: "choice"
    property string date: ""
    property string resTitle: ""
    property string resText: ""
    property var resLines: []
    property bool showInfo: false

    function setInfo(data) {
        resTitle = data.heading || "Result"
        resText = data.message || ""
        resLines = data.details || []
        showInfo = true
    }

    function badInput(msg) {
        setInfo({heading: "Try again", message: msg, details: ["Press Enter after correcting the input."]})
    }

    function dateOk(val) {
        var bits = /^(0?[1-9]|1[0-2])\/(0?[1-9]|[12][0-9]|3[01])$/.exec(val)
        if (!bits)
            return false

        var month = Number(bits[1])
        var day = Number(bits[2])
        return day <= new Date(2000, month, 0).getDate()
    }
    function timeOk(val) {
        return /^(0?[1-9]|1[0-2]):[0-5][0-9]\s?(AM|PM)$/i.test(val)
    }
    function fixDate(val) {
        var parts = val.split("/")
        let firstPart = "";
        let secondPart = "";
        if (parts[0].length === 1) {
            firstPart = "0" + parts[0];
        } else {
            firstPart = parts[0];
        }
        if (parts[1].length === 1) {
            secondPart = "0" + parts[1];
        } else {
            secondPart = parts[1];
        }
        return firstPart + "/" + secondPart;
    }
    function fixTime(val) {
        var bits = /^(0?[1-9]|1[0-2]):([0-5][0-9])\s?(AM|PM)$/i.exec(val)
        return Number(bits[1]) + ":" + bits[2] + " " + bits[3].toUpperCase()
    }
    function choose(num) {
        if (!/^[1-4]$/.test(num)) {
            badInput("Enter a menu number from 1 to 4.")
            return
        }
        if (num === "1") {
            setInfo(Backend.leastCongestedTime())
        } else if (num === "2") {
            step = "date"
            setInfo(Backend.datePrompt())
        } else if (num === "3") {
            setInfo(Backend.weeklyComparison())
        } else {
            setInfo(Backend.benchmark())
        }
    }
    function enter() {
        var val = choice.text.trim()
        if (val.length === 0)
            return
        if (step === "choice") {
            choose(val)
        } else if (step === "date") {
            if (!dateOk(val)) {
                badInput("Use a real month and day in MM/DD format. Do not include a year.")
                choice.text = ""
                return
            }
            date = fixDate(val)
            step = "time"
            setInfo(Backend.timePrompt(date))
        } else {
            if (!timeOk(val)) {
                badInput("Use a time such as 8:30 AM or 4:15 PM.")
                choice.text = ""
                return
            }
            setInfo(Backend.congestionFor(date, fixTime(val)))
            step = "choice"
            date = ""
        }
        choice.text = ""
        choice.forceActiveFocus()
    }
    Rectangle {
        id: card
        width: Math.min(root.width - 64, 572)
        height: 508
        anchors.horizontalCenter: parent.horizontalCenter
        y: 132
        radius: 48
        color: "#001C57"
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
                color: "#001540"
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
                        color: "#001540"
                        border.width: 2
                        border.color: "#00316E"
                    }

                    onAccepted: root.enter()
                }
            }

        }
    }
    Rectangle {
        id: infoBox
        visible: root.showInfo
        width: card.width
        height: 220
        anchors.horizontalCenter: card.horizontalCenter
        anchors.top: card.bottom
        anchors.topMargin: 22
        radius: 21
        color: "#001C57"
        border.width: 1
        border.color: "#00316E"
        clip: true

        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 9

            Text {
                width: parent.width
                text: root.resTitle
                color: "#840505"
                font.family: "Arial"
                font.pixelSize: 17
                font.bold: true
                wrapMode: Text.WordWrap
            }

            Text {
                width: parent.width
                text: root.resText
                color: "#ededea"
                font.family: "Arial"
                font.pixelSize: 14
                lineHeight: 1.25
                wrapMode: Text.WordWrap
            }

            Repeater {
                model: root.resLines

                delegate: Text {
                    required property var modelData
                    width: parent.width
                    text: "- " + modelData
                    color: "#c6c7c2"
                    font.family: "Arial"
                    font.pixelSize: 13
                    wrapMode: Text.WordWrap
                }
            }

        }
    }
}
