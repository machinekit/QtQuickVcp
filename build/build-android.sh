#!/usr/bin/env bash
# Run this script from the projects root directory
set -e

BASEDIR=${PWD}
DISTDIR=${BASEDIR}/dist
BUILDDIR=docker-build-android
PROJECTDIR=QtQuickVcp

cd ..
rm -rf ${BUILDDIR}
cp -r ${PROJECTDIR} ${BUILDDIR}
cd ${BUILDDIR}
git clean -fxd
./build/docker/Android/build.sh
mkdir -p ${DISTDIR}
mv *.apk ${DISTDIR}
mv *.tar.gz ${DISTDIR}
cd ..
rm -rf ${BUILDDIR}
