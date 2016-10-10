#!/bin/bash
# Run this script from QtQuickVcp's root directory

# Build Android package and upload to Google Play Store. Packages will
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
fi

echo "#define REVISION \"${version}\"" > ./src/application/revision.h

# update version number in Android manifest
version_name="$(git describe --tags --abbrev=0)"
version_code="$(git rev-list --first-parent --count HEAD)"
manifest="${PWD}/apps/MachinekitClient/android/AndroidManifest.xml"
sed -i -E "s/(android:versionName=\")([^ ]+)(\")/\1${version_name}\3/" $manifest
sed -i -E "s/(android:versionCode=\")([^ ]+)(\")/\1${version_code}0\3/" $manifest

# Should the Package be uploaded?
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
if [ "${TRAVIS_PULL_REQUEST}" != "false" ]; then
    upload=
fi
platform="armv7"

# write env file
echo -e "upload=${upload}" >> env.list
echo -e "platform=${platform}" >> env.list
echo -e "target=${target}" >> env.list
echo -e "version=${version}" >> env.list
echo -e "release=${release}" >> env.list
echo -e "branch=${branch}" >> env.list

# run build
docker run --env-file ./env.list -i -v "${PWD}:/QtQuickVcp" \
       machinekoder/qtquickvcp-docker-android-armv7:latest \
       /bin/bash -c "/QtQuickVcp/build/Linux/android/Recipe"

if [ "${upload}" ]; then
    # rename binaries
    if [ $release -eq 1 ]; then
        target="MachinekitClient"
    else
        target="MachinekitClient_Development"
    fi
    # Upload apk to Bintray
    ./build/travis/job3_AppImage/bintray_android.sh MachinekitClient*.apk
else
  echo "On branch '$branch' so AppImage will not be uploaded." >&2
fi
