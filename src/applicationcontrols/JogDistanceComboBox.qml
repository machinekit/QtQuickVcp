import QtQuick 2.0
import QtQuick.Controls 1.2

ComboBox {
    property double distance: cbItems.get(jogCombo.currentIndex).distance

    id: jogCombo
    model: ListModel {
            id: cbItems
            ListElement { text: "Continous"; distance: 0 }
            ListElement { text: "1"; distance: 1 }
            ListElement { text: "0.1"; distance: 0.1 }
            ListElement { text: "0.05"; distance: 0.05 }
        }
    // TODO: should come from ini, maybe per axis
}
