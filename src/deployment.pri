include(../paths.pri)
TARGETPATH = $$replace(uri, \\., /)
DESIGNER = designer
DESTDIR = $$OUT_PWD/../../imports/$$TARGETPATH

isEmpty(PLUGIN_VERSION): PLUGIN_VERSION = 1.0

# ========== copy additional files ==========
copyqmlinfra.input = QML_INFRA_FILES
copyqmlinfra.output = $$OUT_PWD/../../imports/$$TARGETPATH/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
!win32: copyqmlinfra.commands = $$QMAKE_MKDIR $$shell_path($$dirname(copyqmlinfra.output)) $$escape_expand(\n\t)
copyqmlinfra.commands +=  $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlinfra.CONFIG += no_link no_clean
copyqmlinfra.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlinfra

copyqmlprivateinfra.input = QML_PRIVATE_INFRA_FILES
copyqmlprivateinfra.output = $$OUT_PWD/../../imports/$$TARGETPATH/Private/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
!win32: copyqmlprivateinfra.commands = $$QMAKE_MKDIR $$shell_path($$dirname(copyqmlprivateinfra.output)) $$escape_expand(\n\t)
copyqmlprivateinfra.commands +=  $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlprivateinfra.CONFIG += no_link no_clean
copyqmlprivateinfra.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlprivateinfra

copyqmldesigner.input = QML_DESIGNER_FILES
copyqmldesigner.output = $$OUT_PWD/../../imports/$$DESIGNER/$$TARGETPATH/designer/
copyqmldesigner.commands = $(COPY_DIR) ${QMAKE_FILE_IN} $$shell_path($$OUT_PWD/../../imports/$$DESIGNER/$$TARGETPATH/designer/)
copyqmldesigner.CONFIG += no_link no_clean
copyqmldesigner.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmldesigner

copyqmlpropertyeditor.input = QML_PROPERTY_EDITOR_FILES
copyqmlpropertyeditor.output = $$OUT_PWD/../../imports/$$DESIGNER/$$TARGETPATH/propertyEditorQmlSources/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
!win32: copyqmlpropertyeditor.commands = $$QMAKE_MKDIR $$shell_path($$dirname(copyqmlpropertyeditor.output)) $$escape_expand(\n\t)
copyqmlpropertyeditor.commands += $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlpropertyeditor.CONFIG += no_link no_clean
copyqmlpropertyeditor.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlpropertyeditor

android: {
    copyjavafiles.input = JAVAFILES
    copyjavafiles.output = $$OUT_PWD/../../src/android/java/
    copyjavafiles.commands = $$QMAKE_MKDIR $$shell_path($$dirname(copyjavafiles.output)) $$escape_expand(\n\t)
    copyjavafiles.commands += $(COPY_DIR) ${QMAKE_FILE_IN} $$shell_path($$OUT_PWD/../../src/android/java/)
    copyjavafiles.CONFIG += no_link no_clean
    copyjavafiles.variable_out = PRE_TARGETDEPS
    QMAKE_EXTRA_COMPILERS += copyjavafiles
}

win32: QMAKE_DEL_FILE = del /q /f
!win32:QMAKE_DEL_FILE = rm -r -f
QMAKE_CLEAN += $$OUT_PWD/../../imports/$$TARGETPATH/

# ========== install additional files ==========
isEmpty(QMLPLUGINDUMP): QMLPLUGINDUMP = 1
equals(QMLPLUGINDUMP, 1): !ios: !android: release: {
    dumppluginqmltypes.CONFIG = no_files no_path
    dumppluginqmltypes.commands = $$dirname(QMAKE_QMAKE)/qmlplugindump -nonrelocatable "$$uri $$PLUGIN_VERSION $$shell_path($$OUT_PWD/../../imports/) > $$shell_path($$OUT_PWD/../../imports/$$TARGETPATH/plugins.qmltypes)"
    # INSTALLS += dumppluginqmltypes

    copypluginqmltypes.CONFIG = no_files no_path
# make sure the directory exists
    !win32: copypluginqmltypes.commands = $$QMAKE_MKDIR $$shell_path($$[QT_INSTALL_QML]/$$TARGETPATH/) $$escape_expand(\n\t)
    !win32: copypluginqmltypes.commands += $$QMAKE_COPY $$shell_path($$OUT_PWD/../../imports/$$TARGETPATH/plugins.qmltypes) $$shell_path($$[QT_INSTALL_QML]/$$TARGETPATH/plugins.qmltypes)
# ignore any errors on Windows
    win32: copypluginqmltypes.commands = xcopy $$shell_path($$OUT_PWD/../../imports/$$TARGETPATH/plugins.qmltypes) $$shell_path($$[QT_INSTALL_QML]/$$TARGETPATH/plugins.qmltypes) /C /E /Q /Y $$escape_expand(\n\t)
    win32: copypluginqmltypes.commands = exit 0
    # INSTALLS += copypluginqmltypes
}

copyqmlinfra_install.files = $$QML_INFRA_FILES
copyqmlinfra_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH
INSTALLS += copyqmlinfra_install

copyqmlprivateinfra_install.files = $$QML_PRIVATE_INFRA_FILES
copyqmlprivateinfra_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH/Private
INSTALLS += copyqmlprivateinfra_install

copyqmldesigner_install.files = $$QML_DESIGNER_FILES
copyqmldesigner_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH
INSTALLS += copyqmldesigner_install

copyqmlpropertyeditor_install.files = $$QML_PROPERTY_EDITOR_FILES
copyqmlpropertyeditor_install.path = $$QTCREATOR_INSTALL_DIR/share/qtcreator/qmldesigner/propertyEditorQmlSources/$$TARGETPATH
INSTALLS += copyqmlpropertyeditor_install

android: {
    copyjavafiles_install.CONFIG = no_files no_path
    copyjavafiles_install.commands += $(COPY_DIR) $$shell_path($$OUT_PWD/../../src/android) $$shell_path($$[QT_INSTALL_ARCHDATA]/src)
    INSTALLS += copyjavafiles_install
}

target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

INSTALLS += target


OTHER_FILES += \
    $$QML_INFRA_FILES \
    $$QML_PRIVATE_INFRA_FILES \
    $$QML_DESIGNER_FILES \
    $$QML_PROPERTY_EDITOR_FILES \
    $$JAVAFILES
