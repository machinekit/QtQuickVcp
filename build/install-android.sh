#!/usr/bin/env bash
# Run this script from the projects root directory
set -e
set -x

app=appdiscover
target=MachinekitClient
if [ "$1" = "${app}" ] || [ -z ${1+x} ]; then
    adb uninstall io.machinekit.$app || true
    adb install dist/$target.apk
    adb shell monkey -p io.machinekit.$app -c android.intent.category.LAUNCHER 1
fi
