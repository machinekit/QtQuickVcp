#! /usr/bin/env sh

APP="MachinekitClient_Development"
LIB="QtQuickVcp_Development"
APPNAME=$(find . -name "${APP}$1")
TAGNAME=$(git rev-parse --short "$GITHUB_SHA")
LIBNAME=$(find . -name "${LIB}$2")
OS_ARCH=$3
DESCRIPTION="${APP} and ${LIB} modules for:
- x64 (64-bit Intel/AMD) Linux systems (Portable AppImages)
- Windows 32bit and 64bit (x86 zip and x64 zip)
- x64 (64-bit Intel/AMD) MacOSX systems
Automated builds of the master development branch. FOR TESTING PURPOSES ONLY!
Extract the contents of the archive to your Qt installation folder to use ${LIB}."

gh release create "$TAGNAME" --notes "$DESCRIPTION" --title "$TAGNAME"
gh release upload "$TAGNAME" "${APPNAME}#${APP} ${OS_ARCH}"
gh release upload "$TAGNAME" "${LIBNAME}#${LIB} ${OS_ARCH}"
