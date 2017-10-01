import QtQuick 2.0

import "../src/applicationcontrols"

UnscaledTestBase {
    id: root

    property var testModel: [{"backangle":0,"comment":"1/8 end mill ","diameter":0.125,"frontangle":0,"id":1,"offset":{"0":0,"1":0,"2":0.511,"3":0,"4":0,"5":0,"6":0,"7":0,"8":0,"a":0,"b":0,"c":0,"u":0,"v":0,"w":0,"x":0,"y":0,"z":0.511},"orientation":0,"pocket":1},{"backangle":0,"comment":"1/16 end mill ","diameter":0.0625,"frontangle":0,"id":2,"offset":{"0":0,"1":0,"2":0.1,"3":0,"4":0,"5":0,"6":0,"7":0,"8":0,"a":0,"b":0,"c":0,"u":0,"v":0,"w":0,"x":0,"y":0,"z":0.1},"orientation":0,"pocket":2},{"backangle":0,"comment":"#7 tap drill ","diameter":0.201,"frontangle":0,"id":3,"offset":{"0":0,"1":0,"2":1.273,"3":0,"4":0,"5":0,"6":0,"7":0,"8":0,"a":0,"b":0,"c":0,"u":0,"v":0,"w":0,"x":0,"y":0,"z":1.273},"orientation":0,"pocket":3},{"backangle":0,"comment":"big tool number ","diameter":0,"frontangle":0,"id":99999,"offset":{"0":0,"1":0,"2":0.1,"3":0,"4":0,"5":0,"6":0,"7":0,"8":0,"a":0,"b":0,"c":0,"u":0,"v":0,"w":0,"x":0,"y":0,"z":0.1},"orientation":0,"pocket":99999},{"backangle":0,"comment":"Added 20131029 ","diameter":0.005,"frontangle":0,"id":4,"offset":{"0":0,"1":0,"2":0,"3":0,"4":0,"5":0,"6":0,"7":0,"8":0,"a":0,"b":0,"c":0,"u":0,"v":0,"w":0,"x":0,"y":0,"z":0},"orientation":0,"pocket":4}]

    ToolTableEditor {
        id: toolTableEditor
        anchors.fill: parent
        toolTable: testModel
    }
}
