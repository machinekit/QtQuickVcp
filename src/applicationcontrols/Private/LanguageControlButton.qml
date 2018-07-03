import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import Machinekit.Application 1.0

Image {
    readonly  property string activeLanguage: getLanguage()

    id: root
    fillMode: Image.PreserveAspectFit
    smooth: true
    source: "qrc:Machinekit/Application/Controls/icons/flag-" + activeLanguage
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
            checkable: true
            checked: root.activeLanguage == "english"
            exclusiveGroup: exclusiveGroup
            onTriggered: root.setLanguage("en")
        }

        MenuItem {
            text: qsTr("German")
            iconSource: "qrc:Machinekit/Application/Controls/icons/flag-german"
            checkable: true
            checked: root.activeLanguage == "german"
            exclusiveGroup: exclusiveGroup
            onTriggered: root.setLanguage("de")
        }

        MenuItem {
            text: qsTr("Russian")
            iconSource: "qrc:Machinekit/Application/Controls/icons/flag-russian"
            checkable: true
            checked: root.activeLanguage == "russian"
            exclusiveGroup: exclusiveGroup
            onTriggered: root.setLanguage("ru")
        }

        ExclusiveGroup {
            id: exclusiveGroup
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
