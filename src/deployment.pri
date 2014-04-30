include(../paths.pri)
DESTDIR = $$OUT_PWD/../../imports/$$TARGETPATH

# ========== install additional files ==========

copyqmlinfra_install.files = $$QML_INFRA_FILES
copyqmlinfra_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH

copyqmldesigner_install.files = $$QML_DESIGNER_FILES
copyqmldesigner_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH

copyqmlpropertyeditor_install.files = $$QML_PROPERTY_EDITOR_FILES
copyqmlpropertyeditor_install.path = $$QTCREATOR_INSTALL_DIR/share/qtcreator/qmldesigner/propertyEditorQmlSources/$$TARGETPATH

target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

INSTALLS += copyqmlinfra_install copyqmldesigner_install copyqmlpropertyeditor_install target

# First, mangle all of my INSTALLS values. I depend on them.
#unset(MANGLED_INSTALLS)
#for(x, INSTALLS):MANGLED_INSTALLS += install_$${x}
#dumplugin.path = $$[QT_INSTALL_QML]/$$TARGETPATH
#dumplugin.commands = $$dirname(QMAKE_QMAKE)/qmlplugindump "$$TARGETPATH 1.0 > plugins.qmltypes"
#dumplugin.depends = $${MANGLED_INSTALLS}
#INSTALLS += dumplugin

copyqmlinfra.input = QML_INFRA_FILES
copyqmlinfra.output = $$OUT_PWD/../../imports/$$TARGETPATH/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
copyqmlinfra.commands = $$QMAKE_MKDIR $$dirname(copyqmlinfra.output) $$escape_expand(\n\t) $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
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
copyqmlpropertyeditor.commands = $$QMAKE_MKDIR $$dirname(copyqmlpropertyeditor.output) $$escape_expand(\n\t) $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlpropertyeditor.CONFIG += no_link no_clean
copyqmlpropertyeditor.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlpropertyeditor


OTHER_FILES += $$QML_INFRA_FILES $$QML_DESIGNER_FILES $$QML_PROPERTY_EDITOR_FILES


