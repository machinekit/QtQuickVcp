TARGET_NAME = $$lower($$NAME)
LANGUAGES = en ru de es
OUTPUT_DIR = $$TRANSLATIONS_OUT_PATH

defineReplace(prependAll) {
    for(a,$$1):result += $$2$${a}$$3
    return($$result)
}

TRANSLATIONS = $$prependAll(LANGUAGES, $${TRANSLATIONS_PATH}/$${TARGET_NAME}_, .ts)
TRANSLATIONS_OUT = $$prependAll(LANGUAGES, $${OUTPUT_DIR}/$${TARGET_NAME}_, .qm)

isEmpty(QMAKE_LUPDATE) {
    win32:QMAKE_LUPDATE = $$shell_path($$[QT_INSTALL_BINS]\lupdate.exe)
    else:QMAKE_LUPDATE = $$shell_path($$[QT_INSTALL_BINS]/lupdate)
}

for(a,TRANSLATIONS): {
    system($$QMAKE_LUPDATE $$join(QML_FILES, " ") -locations none -no-ui-lines -ts $${a})
}

isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$shell_path($$[QT_INSTALL_BINS]\lrelease.exe)
    else:QMAKE_LRELEASE = $$shell_path($$[QT_INSTALL_BINS]/lrelease)
}
updateqm.input = TRANSLATIONS
updateqm.output = $${OUTPUT_DIR}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_MKDIR $$shell_path($$dirname(updateqm.output)) $$escape_expand(\n\t)
updateqm.commands = $$QMAKE_LRELEASE $$shell_path(${QMAKE_FILE_IN}) -qm $$shell_path($${OUTPUT_DIR}/${QMAKE_FILE_BASE}.qm)
updateqm.CONFIG += no_link
updateqm.variable_out = PRE_TARGETDEPS
QMAKE_EXTRA_COMPILERS += updateqm

OTHER_FILES += $${TRANSLATIONS_OUT}
