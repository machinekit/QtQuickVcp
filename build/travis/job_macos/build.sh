#!/bin/bash

set -e
set -x

# do not build mac for PR
if [ ! -z "${TRAVIS_PULL_REQUEST}" ] && [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
  exit 0
fi

# create a full clone
git fetch --unshallow
# find out version number
release=1
git describe --exact-match HEAD 2> /dev/null || release=0
if [ $release -eq 0 ]; then
    date="$(date -u +%Y%m%d%H%M)"

    branch="$TRAVIS_BRANCH"
    [ "$branch" ] || branch="$(git rev-parse --abbrev-ref HEAD)"

    revision="$(echo "$TRAVIS_COMMIT" | cut -c 1-7)"
    [ "$revision" ] || revision="$(git rev-parse --short HEAD)"
    version="${date}-${branch}-${revision}"
else
    version="$(git describe --tags)"
    upload=true
fi

echo "#define REVISION \"${version}\"" > ./src/application/revision.h

##########################################################################
# BUILD QTQUICKVCP
##########################################################################
export PATH="${QT_MACOS}/bin:$PATH"
export LD_LIBRARY_PATH="${QT_MACOS}/lib:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="${QT_MACOS}/plugins"
export QML_IMPORT_PATH="${QT_MACOS}/qml"
export QML2_IMPORT_PATH="${QT_MACOS}/qml"
export QT_QPA_PLATFORM_PLUGIN_PATH="${QT_MACOS}/plugins/platforms"
export QT_INSTALL_PREFIX="${QT_MACOS}"

mkdir -p build.release
cd build.release
qmake -r ..
make
make install

# compress QtQuick module
zipfile="QtQuickVcp.tar.gz"
mkdir qml
mkdir lib
cp -r ${QML_IMPORT_PATH}/Machinekit qml/Machinekit
cp ${QT_INSTALL_PREFIX}/lib/libmachinetalk* lib/
cp /opt/local/lib/libzmq.4* lib/
tar -zcf $zipfile qml lib
# allow access to archive from outside the chroot
chmod a+rwx $zipfile
chmod a+rwx .
ls -lh $zipfile
rm -r qml
rm -r lib

ls
# create Mac disk image
qmldir=${PWD}/../apps/MachinekitClient/
appdir=./apps/MachinekitClient/
cd $appdir
ls
macdeployqt machinekit-client.app -qmldir=$qmldir -dmg -verbose=2
cd ../../
mv ${appdir}/machinekit-client.dmg MachinekitClient.dmg

cd ..

# Should the AppImage be uploaded?
# upload is already on release
if [ "${upload}" != "true" ]; then
    if [ "$1" == "--upload-branches" ] && [ "$2" != "ALL" ]; then
        # User passed in a list of zero or more branches so only upload those listed
        shift
        for upload_branch in "$@" ; do
            [ "$branch" == "$upload_branch" ] && upload=true || true # bypass `set -e`
        done
    else
        # No list passed in (or specified "ALL"), so upload on every branch
        upload=true
    fi
    platform=x64
    # skip pull requests
    if [ ! -z "${TRAVIS_PULL_REQUEST}" ] && [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
        upload=
    fi
fi

if [ "${upload}" ]; then
    # rename binaries
    if [ $release -eq 1 ]; then
        target="QtQuickVcp"
    else
        target="QtQuickVcp_Development"
    fi
    mv build.release/QtQuickVcp.tar.gz ${target}-${version}-MacOSX-${platform}.tar.gz
    # and upload dmg to Bintray
    # ./build/travis/job_macos/bintray_lib.sh ${target}-${version}*.tar.gz

    if [ $release -eq 1 ]; then
        target="MachinekitClient"
    else
        target="MachinekitClient_Development"
    fi
    mv build.release/MachinekitClient.dmg ${target}-${version}-${platform}.dmg
    # ./build/travis/job_macos/bintray_app.sh ${target}*.dmg
else
  echo "On branch '$branch' so dmg will not be uploaded." >&2
fi
