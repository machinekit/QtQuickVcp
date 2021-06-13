#!/bin/bash
# Run this script from QtQuickVcp's root directory

# Build portable Linux AppImages and upload them to Bintray. AppImages will
# always be uploaded unless a list of specific branches is passed in. e.g.:
#    $   build.sh  --upload-branches  master  my-branch-1  my-branch-2
# Builds will be for the native architecture (64 bit) unless another is
# specified for cross-compiling. (e.g. build.sh --i686 or build.sh --armhf)

set -e # exit on error
set -x # echo commands

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

# Build AppImage depending on arch specified in $1 if cross-compiling, else default build x86_64
case "$1" in

  # --armhf )
  #   shift
  #   # build QtQuickVcp inside debian x86-64 multiarch image containing arm cross toolchain and libraries
  #   docker run -i -v "${PWD}:/QtQuickVcp" \
  #     ericfont/musescore:jessie-crosscompile-armhf \
  #     /bin/bash -c \
  #     "/QtQuickVcp/build/Linux+BSD/portable/RecipeDebian --build-only armhf $makefile_overrides"
  #   # then run inside fully emulated arm image for AppImage packing step (which has trouble inside multiarch image)
  #   docker run -i --privileged multiarch/qemu-user-static:register
  #   docker run -i -v "${PWD}:/QtQuickVcp" --privileged \
  #     ericfont/musescore:jessie-packaging-armhf \
  #     /bin/bash -c \
  #     "/QtQuickVcp/build/Linux+BSD/portable/RecipeDebian --package-only armhf"
  #   ;;

  # --i686 )
  #   shift
  #   # Build QtQuickVcp AppImage inside 32-bit x86 Docker image
  #   docker run -i -v "${PWD}:/QtQuickVcp" toopher/centos-i386:centos6 /bin/bash -c \
  #     "linux32 --32bit i386 /QtQuickVcp/build/Linux+BSD/portable/Recipe $makefile_overrides"
  #   ;;


  * )
    [ "$1" == "--x86_64" ] && shift || true
    # Build QtQuickVcp AppImage inside native (64-bit x86) Docker image
    docker run -i -v "${PWD}:/QtQuickVcp" machinekoder/qtquickvcp-docker-linux-x64:latest \
           /bin/bash -c "cd QtQuickVcp; ./build/Linux/portable/Recipe"
    platform="x64"
    ;;
esac

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
    mv build.release/QtQuickVcp.tar.gz ${target}-${version}-Linux-${platform}.tar.gz
    # and upload AppImage to Bintray
    # ./build/travis/job2_AppImage/bintray_lib.sh ${target}-${version}*.tar.gz

    if [ $release -eq 1 ]; then
        target="MachinekitClient"
    else
        target="MachinekitClient_Development"
    fi
    mv build.release/MachinekitClient.AppImage ${target}-${version}-${platform}.AppImage
    # ./build/travis/job2_AppImage/bintray_app.sh ${target}*.AppImage
else
  echo "On branch '$branch' so AppImage will not be uploaded." >&2
fi
