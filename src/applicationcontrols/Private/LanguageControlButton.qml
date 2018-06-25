import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import Machinekit.Application 1.0

Image {
    id: root
    fillMode: Image.PreserveAspectFit
    source: "qrc:Machinekit/Application/Controls/icons/flag-" + getLanguage()
    height: dummyButton.height

    function getLanguage() {
        // uncomment the languages when supported
        var languageMap = {
          "de": "german",
          "ru": "russian",
          //"es": "spanish",
          "en": "english",
          //"uk": "ukranian",
          //"it": "italian",
          //"tu": "turkish",
        }

        var language = ApplicationHelpers.currentLanguage;
        for (var key in languageMap) {
            if (language.indexOf(key)  == 0) {
                return languageMap[key];
            }
        }
        return "english";
    }

    function setLanguage(language) {
        ApplicationHelpers.setLanguage(language);
        restartDialog.open();
    }

    MouseArea {
        anchors.fill: parent
        onClicked: languageMenu.popup()
    }

    Menu {
        id: languageMenu

        MenuItem {
            text: qsTr("English")
            iconSource: "qrc:Machinekit/Application/Controls/icons/flag-english"
            onTriggered: root.setLanguage("en")
        }

        MenuItem {
            text: qsTr("German")
            iconSource: "qrc:Machinekit/Application/Controls/icons/flag-german"
            onTriggered: root.setLanguage("de")
        }

        MenuItem {
            text: qsTr("Russian")
            iconSource: "qrc:Machinekit/Application/Controls/icons/flag-russian"
            onTriggered: root.setLanguage("ru")
        }
    }

    MessageDialog {
        id: restartDialog
        title: qsTr("Restart Application")
        text: qsTr("For the change to take effect, you need to restart the application.\nRestart now?")
        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question
        onYes:  ApplicationHelpers.restartApplication();
    }
}
