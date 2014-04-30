import QtQuick 2.1
import QtQuick.Controls 1.0
import Machinekit.HalRemote.Controls 1.0

HalComboBox {
    name: "comboBox"
    currentIndex: 0
    mode: 1 // Model value mode
    model: ListModel {
        ListElement { text: "Banana"; value: 1.0 }
        ListElement { text: "Apple"; value: 2.5 }
        ListElement { text: "Coconut"; value: 3.2 }
    }
}