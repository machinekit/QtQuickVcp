DESTDIR = $$OUT_PWD/../../imports/$$TARGETPATH

# ========== install additional files ==========

copyqmlinfra_install.files = $$QML_INFRA_FILES
copyqmlinfra_install.path = $$[QT_INSTALL_QML]/$$TARGETPATH
target.path = $$[QT_INSTALL_QML]/$$TARGETPATH
INSTALLS += copyqmlinfra_install target

# First, mangle all of my INSTALLS values. I depend on them.
unset(MANGLED_INSTALLS)
for(x, INSTALLS):MANGLED_INSTALLS += install_$${x}
dumplugin.path = $$[QT_INSTALL_QML]/$$TARGETPATH
dumplugin.commands = $$dirname(QMAKE_QMAKE)/qmlplugindump "$$TARGETPATH 1.0 > plugins.qmltypes"
dumplugin.depends = $${MANGLED_INSTALLS}
#INSTALLS += dumplugin

copyqmlinfra.input = QML_INFRA_FILES
copyqmlinfra.output = $$OUT_PWD/../../imports/$$TARGETPATH/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
copyqmlinfra.commands = $$QMAKE_MKDIR $$dirname(copyqmlinfra.output) $$escape_expand(\n\t) $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copyqmlinfra.CONFIG += no_link no_clean
copyqmlinfra.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += copyqmlinfra

OTHER_FILES += $$QML_INFRA_FILES
