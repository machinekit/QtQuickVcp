#!/bin/bash

set -e
set -x

# do not build mac for PR
#if [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
#  exit 0
#fi

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
fi

echo "#define REVISION \"${version}\"" > ./src/application/revision.h

##########################################################################
# BUILD QTQUICKVCP
##########################################################################
export PATH="${QT_PATH}/bin:$PATH"
export LD_LIBRARY_PATH="${QT_PATH}/lib:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="${QT_PATH}/plugins"
export QML_IMPORT_PATH="${QT_PATH}/qml"
export QML2_IMPORT_PATH="${QT_PATH}/qml"
export QT_QPA_PLATFORM_PLUGIN_PATH="${QT_PATH}/plugins/platforms"
export QT_INSTALL_PREFIX="${QT_PATH}"

mkdir -p build.release
cd build.release
qmake -r ..
make
make install

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
plaform=x64
# skip pull requests
if [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
    upload=
fi

if [ "${upload}" ]; then
    # rename binaries
    if [ $release -eq 1 ]; then
        target="MachinekitClient"
    else
        target="MachinekitClient_Development"
    fi
    mv build.release/MachinekitClient.dmg ${target}-${version}-${platform}.dmg
    # Upload dmg to Bintray
    ./build/travis/job_macos/bintray_app.sh MachinekitClient*.dmg
else
  echo "On branch '$branch' so dmg will not be uploaded." >&2
fi
