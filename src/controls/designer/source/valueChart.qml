import QtQuick 2.1
import QtQuick.Controls 1.0
import Machinekit.Controls 1.0

ValueChart {
    minimumValue: 0
    maximumValue: 200
    valueModel: ValueModel {
        targetValue: 100
        maximumSize: 2000
    }
} 
