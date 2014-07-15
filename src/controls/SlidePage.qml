import QtQuick 2.0

/*!
    \qmltype SlidePage
    \inqmlmodule Machinekit.Controls
    \brief Provides a page element for the \l SlideView.
    \ingroup machinekitcontrols

    \sa SlideView
*/


Rectangle {
    /*! The title of the page. Will be visible in the menu.
    */
    property string title: "SlidePage"

    /*! The source of the icon that will be displayed in the menu.
    */
    property string iconSource: ""

    anchors.fill: parent
    color: systemPalette.window
    opacity: 0

    SystemPalette { id: systemPalette; colorGroup: SystemPalette.Active }

    Behavior on opacity { NumberAnimation { duration: 400; easing.type: Easing.OutQuad } }
}
