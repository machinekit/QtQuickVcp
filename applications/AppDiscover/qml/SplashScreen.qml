import QtQuick 2.0
 
Rectangle {
    id: splash
    z: 100
    property alias loadingProgress: progressRect.percent

    color: "#008080"

    Image {
        source: "../images/body_background_dark.png"
        fillMode: Image.Tile
        anchors.fill: parent
        smooth: true
        visible: true
        opacity: 0.2
    }

    Image {
        id: logoImg
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height*0.1
        source: "../images/logo.png"
        smooth: true
        width: parent.width*0.35
        height: width
        sourceSize.width: width
        sourceSize.height: height
        fillMode: Image.PreserveAspectFit
    }
    Text {
        id: text
        anchors.top: logoImg.bottom
        anchors.topMargin: parent.height*0.04
        anchors.horizontalCenter: parent.horizontalCenter
        text: "machinekit"
        color: "#FFD75E"
        font.pixelSize: parent.width*0.08
    }
    Rectangle {
        id:  barRect
        anchors.top: text.bottom
        anchors.topMargin: parent.height*0.04
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        height: parent.height*0.01
        smooth: true
        border.color: "white"; border.width: 0; radius: 8
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#66343434" }
            GradientStop { position: 1.0; color: "#66000000" }
        }
        Rectangle {
            id: progressRect
            property real percent: 0.5
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width*0.01
            width: parent.width*0.98*percent
            height: parent.height*0.8
            smooth: true
            border.color: "#FFD75E"; border.width: 0; radius: 8
            color: "#FFD75E"
        }
    }




    Behavior on opacity { NumberAnimation { duration: 200 } }
} 
