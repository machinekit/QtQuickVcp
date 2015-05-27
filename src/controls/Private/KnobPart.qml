import QtQuick 2.0

Rectangle {
    property bool isBottom: true

    id: root

    color: Qt.rgba(241, 241, 241, 255)
    border.color: "darkGray"
    gradient: isBottom ? undefined : backGradient
    radius: width/2

    width: 200
    height: 200

    Gradient {
        id: backGradient
        //gradient
        GradientStop { position: 0.0; color: Qt.lighter(root.border.color, 1.3) }
        GradientStop { position: 1.0; color: root.border.color }
    }
}

