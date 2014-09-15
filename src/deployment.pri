include(../paths.pri)
TARGETPATH = $$replace(uri, \\., /)
DESTDIR = $$OUT_PWD/../../imports/$$TARGETPATH

isEmpty(PLUGIN_VERSION): PLUGIN_VERSION = 1.0

# ========== copy additional files ==========

copyqmlinfra.input = QML_INFRA_FILES
copyqmlinfra.output = $$OUT_PWD/../../imports/$$TARGETPATH/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
copyqmlinfra.commands = $$QMAKE_MKDIR $$dirname(copyqmlinfra.output) $$escape_expand(\n\t) \
                        $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlinfra.CONFIG += no_link no_clean
copyqmlinfra.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlinfra

copyqmldesigner.input = QML_DESIGNER_FILES
copyqmldesigner.output = $$OUT_PWD/../../imports/$$TARGETPATH/designer/
copyqmldesigner.commands = $(COPY_DIR) ${QMAKE_FILE_IN} $$OUT_PWD/../../imports/$$TARGETPATH/designer/
copyqmldesigner.CONFIG += no_link no_clean
copyqmldesigner.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmldesigner

copyqmlpropertyeditor.input = QML_PROPERTY_EDITOR_FILES
copyqmlpropertyeditor.output = $$OUT_PWD/../../imports/$$TARGETPATH/propertyEditorQmlSources/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
copyqmlpropertyeditor.commands = $$QMAKE_MKDIR $$dirname(copyqmlpropertyeditor.output) $$escape_expand(\n\t) \
                                 $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlpropertyeditor.CONFIG += no_link no_clean
copyqmlpropertyeditor.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlpropertyeditor

!android: !ios: {
    QMAKE_POST_LINK += $$dirname(QMAKE_QMAKE)/qmlplugindump "$$uri $$PLUGIN_VERSION $$OUT_PWD/../../imports/ > $$OUT_PWD/../../imports/$$TARGETPATH/plugins.qmltypes"
}

QMAKE_CLEAN += -r $$OUT_PWD/../../imports/$$TARGETPATH/

# ========== install additional files ==========
!android: !ios: {
    copypluginqmltypes.CONFIG = no_files no_path
    copypluginqmltypes.extra = $$QMAKE_MKDIR $$[QT_INSTALL_QML]/$$TARGETPATH/ $$escape_expand(\n\t) \
                               $$QMAKE_COPY $$OUT_PWD/../../imports/$$TARGETPATH/plugins.qmltypes $$[QT_INSTALL_QML]/$$TARGETPATH/plugins.qmltypes
    INSTALLS += copypluginqmltypes
}

copyqmlinfra_install.files = $$QML_INFRA_FILES
copyqmlinfra_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH
INSTALLS += copyqmlinfra_install

copyqmldesigner_install.files = $$QML_DESIGNER_FILES
copyqmldesigner_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH
INSTALLS += copyqmldesigner_install

copyqmlpropertyeditor_install.files = $$QML_PROPERTY_EDITOR_FILES
copyqmlpropertyeditor_install.path = $$QTCREATOR_INSTALL_DIR/share/qtcreator/qmldesigner/propertyEditorQmlSources/$$TARGETPATH
INSTALLS += copyqmlpropertyeditor_install

target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

INSTALLS += target


OTHER_FILES += $$QML_INFRA_FILES $$QML_DESIGNER_FILES $$QML_PROPERTY_EDITOR_FILES
