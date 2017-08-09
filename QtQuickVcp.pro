# QtQuickVcp project

defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

!minQtVersion(5, 4, 0) {
    message("Cannot build QtQuickVcp with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.4.0.")
}

TEMPLATE = subdirs

SUBDIRS += 3rdparty src apps # examples

src.depends = 3rdparty
apps.depends = src

CONFIG(debug, debug|release) {
    SUBDIRS += tests
    tests.depends = src
}

include(qtquickvcp_version.pri)
include(paths.pri)
include(doc/doc.pri)

CONFIG(debug, debug|release) {
    message("running in DEBUG mode")
}
else {
    message("running in RELEASE mode")
}
