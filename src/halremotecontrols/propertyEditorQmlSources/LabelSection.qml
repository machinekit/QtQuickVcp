import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    StandardTextSection {
        useLineEdit: true
        showIsWrapping: true
        showVerticalAlignment: true
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Text Color")

        ColorEditor {
            caption: qsTr("Text Color")
            backendendValue: backendValues.color
            supportGradient: false
        }

    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Style Color")

        ColorEditor {
            caption: qsTr("Style Color")
            backendendValue:  backendValues.styleColor
            supportGradient: false
        }
    }

   FontSection {
       showStyle: true
   }
} 
