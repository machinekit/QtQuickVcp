/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import Machinekit.Application 1.0

ApplicationItem {
    property var toolTable: d.ready ? status.io.toolTable : []
    property int decimals: (d.distanceUnits === "mm") ? 3 : 4
    property color modifiedColor: "orange"
    property color errorColor: "#FF9999"

    readonly property alias errored: d.errored
    readonly property alias modified: d.modified
    readonly property bool itemSelected: table.selection.count > 0

    id: root
    width: 800
    height: 600

    onToolTableChanged: d.updateToolTableCopy(toolTable)

    function addRow() { d.addRow(); }
    function removeRow() { d.deleteRow(); }
    function resetModifications() { d.updateToolTableCopy(root.toolTable); }
    function updateToolTable() { d.commitToolTable(); }

    QtObject {
        id: d
        readonly property bool ready: root.status.synced
        property string distanceUnits: root.helper.ready ? root.helper.distanceUnits: "mm"

        readonly property int toolIdColumnWidth: 60
        readonly property int offsetColumnWidth: 45
        readonly property int parameterWidth: 90
        readonly property int commentWidth: 160
        readonly property int minimumToolId: 0
        readonly property int maximumToolId: 99999
        readonly property int minimumPocketId: 1
        readonly property int maximumPocketId: 99999
        readonly property int minimumPosition: 0
        readonly property int maximumPosition: 9

        property var updatedToolTable: []
        property var changeMatrix: []
        property var toolIdErrors: []
        property bool modified: false
        property bool errored: false

        property var cellReferenceTable: []
        property bool cellReferenceTableNeedsUpdate: false

        function addCellReference(row, column, reference) {
            d.ensureCellReferenceTableSize(row, column, d.cellReferenceTable);
            d.cellReferenceTable[row][column] = reference;
            d.cellReferenceTableNeedsUpdate = true;
        }

        function getCellReference(row, column) {
            if ((row === -1) || (column === -1)) {
                return null;
            }

            var table = d.cellReferenceTable;
            if ((table.length <= row) ||
                    !table[row] || (table[row].length <= column)) {
                return null;
            }

            return d.cellReferenceTable[row][column];
        }

        function ensureCellReferenceTableSize(row, column, table) {
            while ((table.length - 1) < row) {
                table.push([]);
            }

            var currentRow = table[row];
            while (currentRow.length - 1 < column) {
                currentRow.push(null);
            }
        }

        function activateCellReferenceTable() {
            if (d.cellReferenceTableNeedsUpdate) {
                d.cellReferenceTable = d.cellReferenceTable;
                d.cellReferenceTableNeedsUpdate = false;
            }
        }

        function updateToolTableCopy(origin) {
            d.updatedToolTable = JSON.parse(JSON.stringify(origin));
            d.updateChangeMatrix(origin);

            var newToolIdErrors = [];
            for (var x in origin) {
                newToolIdErrors.push(false);
            }
            toolIdErrors = newToolIdErrors;

            d.modified = false;
        }

        function updateChangeMatrix(origin) {
            var newChangeMatrix = [];
            for (var i = 0; i < origin.length; ++i) {
                var row = []
                for (var j = 0; j < table.columnCount; ++j) {
                    row.push(false);
                }
                newChangeMatrix.push(row);
            }
            d.changeMatrix = newChangeMatrix;
        }

        function updateToolData(row, column, data) {
            var role = table.getColumn(column).role;
            var oldData = updatedToolTable[row][role];
            if (data != oldData) {
                updatedToolTable[row][role] = data;
                markCellAsModified(row, column);
            }
            d.checkToolTableIngredity();
        }

        function updateAxisToolData(row, column, data, axis) {
            var role = table.getColumn(column).role;
            var oldData = updatedToolTable[row][role][axis];
            if (data != oldData) {
                updatedToolTable[row][role][axis] = data;
                markCellAsModified(row, column);
            }
            d.checkToolTableIngredity();
        }

        function markCellAsModified(row, column) {
            d.changeMatrix[row][column] = true;
            d.changeMatrix = d.changeMatrix; // trigger update
            d.modified = true;
            d.checkToolTableIngredity();
        }

        function verifyToolId(data) {
            var error = false;

            var newToolIdErrors = new Array(data.length);
            for (var i in data) {
                for (var j = 0; j < i; ++j) {
                    if (data[i]["id"] == data[j]["id"]) {
                        newToolIdErrors[i] = true;
                        newToolIdErrors[j] = true;
                        error = true;
                    }
                }
            }
            toolIdErrors = newToolIdErrors;
            return error;
        }

        function checkToolTableIngredity()
        {
            d.errored = verifyToolId(d.updatedToolTable);
        }

        function prepareUpdatedToolTable() {
            for (var i in d.updatedToolTable) {
                var offset = d.updatedToolTable[i]['offset'];
                offset['x'] = offset[0];
                offset['y'] = offset[1];
                offset['z'] = offset[2];
                offset['a'] = offset[3];
                offset['b'] = offset[4];
                offset['c'] = offset[5];
                offset['u'] = offset[6];
                offset['v'] = offset[7];
                offset['w'] = offset[8];
            }
        }

        function selectRow(row) {
            table.selection.clear();
            table.selection.select(row);
            // we must update the reference table here for performance reasons
            d.activateCellReferenceTable();
        }

        function addRow() {
            var newRow = {}
            newRow["id"] = 0;
            newRow["pocket"] = 1;
            newRow["offset"] = { 0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0, 5: 0.0, 6: 0.0, 7: 0.0, 8: 0.0 }
            newRow["diameter"] = 0.0;
            newRow["frontangle"] = 0.0
            newRow["backangle"] = 0.0;
            newRow["orientation"] = 0;
            newRow["comment"] = "";
            d.updatedToolTable.push(newRow);
            d.updateChangeMatrix(d.updatedToolTable);
            d.updatedToolTable = d.updatedToolTable;
            d.modified = true;
            d.checkToolTableIngredity();
        }

        function deleteRow() {
            if (table.selection.count === 0) {
                return;
            }

            table.selection.forEach( function removeRow(row) {
                root.forceActiveFocus();
                d.updatedToolTable.splice(row, 1);
                d.updatedToolTable = d.updatedToolTable;
            });
            d.modified = true;
            d.checkToolTableIngredity();

            if (table.rowCount > 0) {
                table.selection.select(table.rowCount - 1);
            }
        }

        function commitToolTable() {
            prepareUpdatedToolTable();
            root.command.updateToolTable(d.updatedToolTable);
            root.command.loadToolTable();
        }
    }

    TableView {
        id: table
        anchors.fill: parent

        model: d.updatedToolTable
        itemDelegate: null

        TableViewColumn {
            role: "id"
            title: qsTr("Tool ID")
            width: d.toolIdColumnWidth
            delegate: toolIdEdit
        }

        TableViewColumn {
            role: "pocket"
            title: qsTr("Pocket")
            width: d.toolIdColumnWidth
            delegate: pocketEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("X")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("Y")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("Z")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("A")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("B")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("C")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("U")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("V")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "offset"
            title: qsTr("W")
            width: d.offsetColumnWidth
            delegate: offsetEdit
        }

        TableViewColumn {
            role: "diameter"
            title: qsTr("Diameter")
            width: d.parameterWidth
            delegate: doubleEdit
        }

        TableViewColumn {
            role: "frontangle"
            title: qsTr("Front Angle")
            width: d.parameterWidth
            delegate: doubleEdit
        }

        TableViewColumn {
            role: "backangle"
            title: qsTr("Back Angle")
            width: d.parameterWidth
            delegate: doubleEdit
        }

        TableViewColumn {
            role: "orientation"
            title: qsTr("Orientation")
            width: d.parameterWidth
            delegate: orientationEdit
        }

        TableViewColumn {
            role: "comment"
            title: qsTr("Comment")
            width: d.commentWidth
            delegate: textEdit
        }
    }

    Component {
        id: toolIdEdit

        Rectangle {
            readonly property bool hasChanged: (d.changeMatrix[styleData.row] && d.changeMatrix[styleData.row][styleData.column]) ? true : false
            readonly property bool hasError: d.toolIdErrors[styleData.row] ? true : false
            color: hasError ? root.errorColor : (hasChanged ? root.modifiedColor : "transparent")

            TextInput {
                id: textInput
                anchors.fill: parent
                text: Number(styleData.value).toLocaleString(Qt.locale(), 'f', 0)
                color: styleData.textColor
                validator: IntValidator { bottom: d.minimumToolId; top: d.maximumToolId }

                onEditingFinished: d.updateToolData(styleData.row, styleData.column,  Number.fromLocaleString(Qt.locale(), text));
                onActiveFocusChanged: if (activeFocus) {
                                          d.selectRow(styleData.row); textInput.selectAll();
                                      }

                KeyNavigation.tab: d.getCellReference(styleData.row, styleData.column + 1)
                KeyNavigation.backtab: d.getCellReference(styleData.row - 1, table.columnCount - 1)
                KeyNavigation.up: d.getCellReference(styleData.row - 1, styleData.column)
                KeyNavigation.down: d.getCellReference(styleData.row + 1, styleData.column)
                Component.onCompleted: d.addCellReference(styleData.row, styleData.column, textInput)
            }
        }
    }

    Component {
        id: pocketEdit

        Rectangle {
            color: (d.changeMatrix[styleData.row] && d.changeMatrix[styleData.row][styleData.column]) ? root.modifiedColor : "transparent"

            TextInput {
                id: textInput
                anchors.fill: parent
                text: Number(styleData.value).toLocaleString(Qt.locale, 'f', 0)
                color: styleData.textColor
                validator: IntValidator { bottom: d.minimumPocketId; top: d.maximumPocketId }

                onEditingFinished: d.updateToolData(styleData.row, styleData.column,  Number.fromLocaleString(Qt.locale(), text));
                onActiveFocusChanged: if (activeFocus) {
                                          d.selectRow(styleData.row); textInput.selectAll();
                                      }

                KeyNavigation.tab: d.getCellReference(styleData.row, styleData.column + 1)
                KeyNavigation.backtab: d.getCellReference(styleData.row, styleData.column - 1)
                KeyNavigation.up: d.getCellReference(styleData.row - 1, styleData.column)
                KeyNavigation.down: d.getCellReference(styleData.row + 1, styleData.column)
                Component.onCompleted: d.addCellReference(styleData.row, styleData.column, textInput)
            }
        }
    }

    Component {
        id: offsetEdit

        Rectangle {
            color: (d.changeMatrix[styleData.row] && d.changeMatrix[styleData.row][styleData.column]) ? root.modifiedColor : "transparent"

            TextInput {
                id: textInput
                anchors.fill: parent
                readonly property int offset: 2
                readonly property int axis: styleData.column - offset
                text: Number(styleData.value[axis]).toLocaleString(Qt.locale(), 'f', root.decimals)
                color: styleData.textColor
                validator: DoubleValidator { }

                onEditingFinished: d.updateAxisToolData(styleData.row, styleData.column, Number.fromLocaleString(Qt.locale(), text), axis)
                onActiveFocusChanged: if (activeFocus) {
                                          d.selectRow(styleData.row); textInput.selectAll();
                                      }

                KeyNavigation.tab: d.getCellReference(styleData.row, styleData.column + 1)
                KeyNavigation.backtab: d.getCellReference(styleData.row, styleData.column - 1)
                KeyNavigation.up: d.getCellReference(styleData.row - 1, styleData.column)
                KeyNavigation.down: d.getCellReference(styleData.row + 1, styleData.column)
                Component.onCompleted: d.addCellReference(styleData.row, styleData.column, textInput)
            }
        }
    }

    Component {
        id: doubleEdit

        Rectangle {
            color: (d.changeMatrix[styleData.row] && d.changeMatrix[styleData.row][styleData.column]) ? root.modifiedColor : "transparent"

            TextInput {
                id: textInput
                anchors.fill: parent
                text: Number(styleData.value).toLocaleString(Qt.locale(), 'f', root.decimals)
                color: styleData.textColor
                validator: DoubleValidator { bottom: 0.0 }

                onEditingFinished: d.updateToolData(styleData.row, styleData.column, Number.fromLocaleString(Qt.locale(), text))
                onActiveFocusChanged: if (activeFocus) {
                                          d.selectRow(styleData.row); textInput.selectAll();
                                      }

                KeyNavigation.tab: d.getCellReference(styleData.row, styleData.column + 1)
                KeyNavigation.backtab: d.getCellReference(styleData.row, styleData.column - 1)
                KeyNavigation.up: d.getCellReference(styleData.row - 1, styleData.column)
                KeyNavigation.down: d.getCellReference(styleData.row + 1, styleData.column)
                Component.onCompleted: d.addCellReference(styleData.row, styleData.column, textInput)
            }
        }
    }

    Component {
        id: orientationEdit

        Rectangle {
            color: (d.changeMatrix[styleData.row] && d.changeMatrix[styleData.row][styleData.column]) ? root.modifiedColor : "transparent"

            TextInput {
                id: textInput
                anchors.fill: parent
                text: Number(styleData.value).toLocaleString(Qt.locale(), 'f', 0)
                color: styleData.textColor
                validator: IntValidator { bottom: d.minimumPosition; top: d.maximumPosition }

                onEditingFinished: d.updateToolData(styleData.row, styleData.column,  Number.fromLocaleString(Qt.locale(), text))
                onActiveFocusChanged: if (activeFocus) {
                                          d.selectRow(styleData.row); textInput.selectAll();
                                      }

                KeyNavigation.tab: d.getCellReference(styleData.row, styleData.column + 1)
                KeyNavigation.backtab: d.getCellReference(styleData.row, styleData.column - 1)
                KeyNavigation.up: d.getCellReference(styleData.row - 1, styleData.column)
                KeyNavigation.down: d.getCellReference(styleData.row + 1, styleData.column)
                Component.onCompleted: d.addCellReference(styleData.row, styleData.column, textInput)
            }
        }
    }

    Component {
        id: textEdit
        Rectangle {
            color: (d.changeMatrix[styleData.row] && d.changeMatrix[styleData.row][styleData.column]) ? root.modifiedColor : "transparent"

            TextInput {
                id: textInput
                anchors.fill: parent
                text: styleData.value
                color: styleData.textColor

                onEditingFinished: d.updateToolData(styleData.row, styleData.column, text)
                onActiveFocusChanged: if (activeFocus) {
                                          d.selectRow(styleData.row); textInput.selectAll();
                                      }

                KeyNavigation.tab: d.getCellReference(styleData.row + 1, 0)
                KeyNavigation.backtab: d.getCellReference(styleData.row, styleData.column - 1)
                KeyNavigation.up: d.getCellReference(styleData.row - 1, styleData.column)
                KeyNavigation.down: d.getCellReference(styleData.row + 1, styleData.column)
                Component.onCompleted: d.addCellReference(styleData.row, styleData.column, textInput)
            }
        }
    }
}
