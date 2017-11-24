#!/bin/bash
# Run this script from QtQuickVcp's root directory

# QtQuickVcp Test build

set -e # exit on error
set -x # echo commands

# Run tests depending on arch specified in $1 if cross-compiling, else default build x86_64
case "$1" in
  * )
    [ "$1" == "--x86_64" ] && shift || true
    # Build QtQuickVcp AppImage inside native (64-bit x86) Docker image
    docker run -i -v "${PWD}:/QtQuickVcp" machinekoder/qtquickvcp-docker-linux-x64:latest \
           /bin/bash -c "cd QtQuickVcp; ./build/Linux/test/Recipe"
    platform="x64"
    ;;
esac
