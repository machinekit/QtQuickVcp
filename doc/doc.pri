OTHER_FILES += \
    doc/*.qdoc \
    doc/reference/*.qdoc \
    doc/qtquickvcp.qdocconf \
    doc/config/qtquickvcp-project.qdocconf \
    src/controls/doc/*.qdoc \
    src/halremote/doc/*.qdoc \
    src/halremotecontrols/doc/*.qdoc \
    src/pathview/doc/*.qdoc \
    src/videoview/doc/*.qdoc

defineReplace(targetPath) {
    return($$replace(1, /, $$QMAKE_DIR_SEP))
}

QDOC_BIN = $$targetPath($$[QT_INSTALL_BINS]/qdoc)
QDOC_MAINFILE = $$PWD/qtquickvcp.qdocconf

HELPGENERATOR = $$targetPath($$[QT_INSTALL_BINS]/qhelpgenerator)

VERSION_TAG = $$replace(QTQUICKVCP_VERSION, "[-.]", )

HTML_DOC_PATH=$$OUT_PWD/doc/html

equals(QMAKE_DIR_SEP, /) {   # unix, mingw+msys
    QDOC = SRCDIR=$$relative_path($$PWD/../..,$$PWD) OUTDIR=$$HTML_DOC_PATH QTDOCDIR=$$QT_DOC_DIR QTQUICKVCP_VERSION=$$QTQUICKVCP_VERSION QTQUICKVCP_VERSION_TAG=$$VERSION_TAG QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS] $$QDOC_BIN
} else:win32-g++* {   # just mingw
    # The lack of spaces in front of the && is necessary!
    QDOC = set SRCDIR=$$absolute_path($$PWD/..)&& set OUTDIR=$$HTML_DOC_PATH&& set QTDOCDIR=$$QT_DOC_DIR&& set QTQUICKVCP_VERSION=$$QTQUICKVCP_VERSION&& set QTQUICKVCP_VERSION_TAG=$$VERSION_TAG&& set QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS]&& $$QDOC_BIN
} else {   # nmake
    QDOC = set SRCDIR=$$absolute_path($$PWD/..) $$escape_expand(\\n\\t) \
           set OUTDIR=$$HTML_DOC_PATH $$escape_expand(\\n\\t) \
           set QTDOCDIR=$$QT_DOC_DIR $$escape_expand(\\n\\t) \
           set QTQUICKVCP_VERSION=$$QTQUICKVCP_VERSION $$escape_expand(\\n\\t) \
           set QTQUICKVCP_VERSION_TAG=$$VERSION_TAG $$escape_expand(\\n\\t) \
           set QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS] $$escape_expand(\\n\\t) \
           $$QDOC_BIN
}

QHP_FILE = $$HTML_DOC_PATH/qtquickvcp.qhp
QCH_FILE = $$OUT_PWD/doc/qtquickvcp.qch

HELP_DEP_FILES = $$PWD/qtquickvcp.qdoc \
                 $$PWD/config/compat-qt4.qdocconf \
                 $$PWD/config/macros.qdocconf \
                 $$PWD/config/qt-cpp-ignore.qdocconf \
                 $$PWD/config/qt-html-templates.qdocconf \
                 $$PWD/config/qt-html-default-styles.qdocconf \
                 $$QDOC_MAINFILE

html_docs.commands = $$QDOC $$QDOC_MAINFILE
html_docs.depends += $$HELP_DEP_FILES

qch_docs.commands = $$HELPGENERATOR -o $$shell_quote($$QCH_FILE) $$QHP_FILE
qch_docs.depends += html_docs

inst_qch_docs.files = $$QCH_FILE
inst_qch_docs.path = $$[QT_INSTALL_DOCS]
inst_qch_docs.CONFIG += no_check_exist no_default_install
INSTALLS += inst_qch_docs

inst_html_docs.files = $$HTML_DOC_PATH
inst_html_docs.path = $$inst_qch_docs.path
inst_html_docs.CONFIG += no_check_exist no_default_install directory
INSTALLS += inst_html_docs

install_docs.depends = install_inst_qch_docs install_inst_html_docs
QMAKE_EXTRA_TARGETS += install_docs

docs.depends = qch_docs
QMAKE_EXTRA_TARGETS += html_docs qch_docs docs

fixnavi.commands = \
    cd $$targetPath($$PWD) && \
    perl fixnavi.pl -Dqcmanual -Dqtquick \
        qbs.qdoc
QMAKE_EXTRA_TARGETS += fixnavi
