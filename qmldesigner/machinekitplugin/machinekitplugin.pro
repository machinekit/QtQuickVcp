DEFINES += MACHINEKIT_LIBRARY

TARGET = machinekitplugin
TEMPLATE = lib
CONFIG += plugin

include(../../paths.pri)

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=$$QTCREATOR_SOURCE_DIR

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=$$QTCREATOR_INSTALL_DIR

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = MachinekitProject

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = MachinekitPlugin
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    #coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

SOURCES += $$PWD/machinekitplugin.cpp

HEADERS += $$PWD/machinekitplugin.h  $$QTCREATOR_SOURCES/src/plugins/qmldesigner/designercore/include/iwidgetplugin.h

RESOURCES += $$PWD/machinekitplugin.qrc

OTHER_FILES += $$PWD/machinekit.metainfo

DESTDIR = $$IDE_BUILD_TREE/lib/qtcreator/qmldesigner/

include($$QTCREATOR_SOURCES/qtcreator.pri)
include ($$QTCREATOR_SOURCES/src/plugins/qmldesigner/designercore/iwidgetplugin.pri)
#include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
