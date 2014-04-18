import QtQuick 2.0
import Machinekit.Controls 1.0

Rectangle {
    id: main

    property double maxVelocity: 100.0
    property double stepSize: 10.0
    property bool autoCenter: true
    property double xVelocity: 0
    readonly property double yVelocity: 0

    Binding { target: main; property: "xVelocity"; value: (Math.floor(((control.x + maxControlX()) / Math.abs(maxControlX()) * maxVelocity) / stepSize) * stepSize)}
    //Binding { target: control; property: "x"; value: Math.pow(main.xVelocity/stepSize,2) * stepSize / maxVelocity * Math.abs(maxControlX()) - maxControlX()}

    Binding { target: main; property: "yVelocity"; value: (Math.floor(((control.y + maxControlY()) / Math.abs(maxControlY()) * maxVelocity) / stepSize) * stepSize)}

    function centeredX()
    {
        return main.width/2 - control.width/2
    }

    function centeredY()
    {
        return main.height/2 - control.height/2
    }

    function maxX()
    {
        //var maxRadius = main.width/2

        //return Math.sqrt(Math.pow(maxRadius, 2)-Math.pow(y, 2))
        return main.width - control.width
    }

    function maxY()
    {
        return main.height - control.height
    }

    function maxControlX()
    {
        return control.width/2 - main.width/2
    }

    function maxControlY()
    {
        return control.height/2 - main.height/2
    }

    function calculateNewX()
    {
        var newControlX = 0

        newControlX = controlArea.mouseX - control.width/2
        if (newControlX > maxX())
        {
            newControlX = maxX()
        }
        else if (newControlX < 0)
        {
            newControlX = 0
        }

        control.x = newControlX
    }

    function calculateNewY()
    {
        var newControlY = 0

        newControlY = controlArea.mouseY - control.height/2
        if (newControlY > maxY())
        {
            newControlY = maxY()
        }
        else if (newControlY < 0)
        {
            newControlY = 0
        }

        control.y = newControlY
    }

    width: 150; height: 150
    color: "#00000000"

    Rectangle {
        id: circle1

        width: parent.width; height: parent.height
        anchors.centerIn: parent
        radius: width / 2
        clip: true
        color: "#999999"

        Rectangle {
            id: circle2

            width: main.width * 0.8; height: main.width * 0.8
            anchors.centerIn: parent
            radius: width / 2
            clip: true
            color: "#FFFFFF"

            Rectangle {
                id: circle3

                width: main.width * 0.6; height: main.width * 0.6
                anchors.centerIn: parent
                radius: width / 2
                clip: true
                color: "#999999"
            }
        }
    }

    Rectangle {
        id: control

        property bool movable: false

        width: main.width * 0.3; height: main.width * 0.3
        x: centeredX(); y: centeredY(); z: 1
        //anchors.centerIn: parent
        radius: width / 2
        clip: true
        color: {
            if (!movable)
                "#FF0000"
            else
                "#0000FF"
        }


    }

    MouseArea {
        id: controlArea

        anchors.fill: parent

        onPressed: control.movable = true
        onReleased: {
            control.movable = false
            if (main.autoCenter)
            {
                control.x = centeredX()
                control.y = centeredY()
            }
        }
        onMouseXChanged: {
            if (control.movable)
            {
                calculateNewX()
            }
        }
        onMouseYChanged: {
            if (control.movable)
            {
                calculateNewY()
            }
        }
    }

    Line {
        id: line

        x1: main.width/2; y1: main.height/2
        x2: control.x + control.width/2; y2: control.y + control.height/2
        lineWidth: 14
    }

    /*onYVelocityChanged: {
        console.log("Y:")
        console.log(yVelocity)
    }

    onXVelocityChanged: {
        console.log("X:")
        console.log(xVelocity)
    }*/
}
