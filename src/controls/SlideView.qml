import QtQuick 2.0
import QtQuick.Window 2.0

/*!
    \qmltype SlideView
    \inqmlmodule Machinekit.Controls
    \brief Provides a stacked based view with sliding menus.
    \ingroup machinekitcontrols

    The following example will create a slide view with two sliding pages.

    \qml
    SlideView {
        id: slideView

        SlidePage {
            id: slidePage1
            title: qsTr("Page 1")
        }

        SlidePage {
            id: slidePage1
            title: qsTr("Page 2")
        }
    }
    \endqml

    \sa SlidePage
*/

Item {
    id: root

    /*! Whether the menu is currently visible or not.
    */
    property bool menuVisible: false

    /*! Whether the menu should be automatically hidden if an item from the menu was selected.

        By default this is set to \c true.
    */
    property bool autoHideMenu: true

    /*! The current index of the view.
    */
    property alias currentIndex: menuList.currentIndex

    /*! Shows the menu.
    */
    function showMenu() {
        menuVisible = true
    }

    /*! Hides the menu.
    */
    function hideMenu() {
        menuVisible = false
    }

    SystemPalette { id: systemPalette; colorGroup: SystemPalette.Active }

    MouseArea {
        property int startX: 0
        property int dragX: pressed ? mouseX - startX : 0
        property int dragThreshold: menuView.width * 0.1

        id: mouseArea
        anchors.fill: parent

        onPressed: {
            startX = mouseX
        }
        onReleased: {
            if (dragX > dragThreshold)
                menuVisible = true
            else if (dragX < -dragThreshold)
                menuVisible = false
        }
    }

    Rectangle {
       id: menuView
       anchors.left: parent.left
       anchors.top: parent.top
       anchors.bottom: parent.bottom
       width: Screen.pixelDensity * 50
       color: systemPalette.base

       opacity: viewTranslate.x/width
       enabled: opacity != 0

       Rectangle {
           id: shadow1
           anchors.top: parent.top
           anchors.bottom: parent.bottom
           x: viewTranslate.x-width
           width: Screen.pixelDensity*1.6
           color: systemPalette.shadow
           opacity: 0.3
       }

       Rectangle {
           id: shadow2
           anchors.top: shadow1.top
           anchors.bottom: shadow1.bottom
           anchors.right: shadow1.right
           width: shadow1.width * 0.6
           color: systemPalette.shadow
           opacity: 0.3
       }

       Rectangle {
           id: shadow3
           anchors.top: shadow1.top
           anchors.bottom: shadow1.bottom
           anchors.right: shadow1.right
           width: shadow2.width * 0.6
           color: systemPalette.shadow
           opacity: 0.3
       }

       ListView {
           id: menuList
           anchors.fill: parent
           model: normalView.children

           delegate: Component {
                   Rectangle {
                       property bool isCurrentItem: ListView.isCurrentItem

                       id: rect
                       anchors.left: parent.left
                       anchors.right: parent.right
                       height: Screen.pixelDensity * 10
                       color: rect.isCurrentItem ? systemPalette.highlight : "transparent"
                       ListView.onCurrentItemChanged: menuList.model[index].opacity = ListView.isCurrentItem * 1.0

                       Text {
                           anchors.fill: parent
                           text: title
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           color: rect.isCurrentItem ? systemPalette.highlightedText : systemPalette.text
                       }

                       Canvas {
                           anchors.top: parent.top
                           anchors.bottom: parent.bottom
                           anchors.right: parent.right
                           width: parent.width * 0.1
                           contextType: "2d"
                           opacity: menuList.model[index].opacity

                           onPaint: {
                               getContext("2d")
                               context.reset();

                               context.fillStyle = menuList.model[index].color
                               context.beginPath();
                               context.moveTo(0,height/2)
                               context.lineTo(width, 0);
                               context.lineTo(width, height);
                               context.fill()
                               context.closePath();
                           }
                       }

                       Image {
                           visible: menuList.model[index].iconSource !== ""
                           anchors.left: parent.left
                           anchors.top: parent.top
                           anchors.bottom: parent.bottom
                           anchors.margins: parent.height * 0.2
                           width: height
                           source: menuList.model[index].iconSource
                           fillMode: Image.PreserveAspectFit
                       }

                       MouseArea {
                           anchors.fill: parent
                           hoverEnabled: false
                           onClicked: {
                               if (root.autoHideMenu)
                                   menuVisible = false
                               menuList.currentIndex = index
                           }
                       }
               }
           }
       }

    }

    Rectangle {
       id: normalView
       anchors.fill: parent
       color: systemPalette.window

       transform: Translate {
                   id: viewTranslate
                   x: menuVisible ? Math.max(0, menuView.width + Math.min(0, mouseArea.dragX)) :  Math.min(menuView.width, Math.max(0, mouseArea.dragX))
                   Behavior on x { NumberAnimation { duration: 400; easing.type: Easing.OutQuad } }
               }
    }

    onChildrenChanged: {
       var items = root.children
       var filteredItems = []
       var i

       for (i = items.length-1; i >= 0; --i) {
           if (items[i] === menuView )
               continue

           if (items[i] === mouseArea )
               continue

           if (items[i] === normalView )
               continue

           filteredItems.push(items[i])
       }

       for (i = filteredItems.length-1; i >= 0; --i) {
           filteredItems[i].parent = normalView
       }
    }
}
