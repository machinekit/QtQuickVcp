setlocal

appveyor DownloadFile http://buildbot.roessler.systems/files/qt-bin/Qt-5.7-Android-armv7.tar.bz2 -Filename qt5.zip

cd %APPVEYOR_BUILD_FOLDER%
mkdir build.release
cd build.release
qmake ..
nmake
nmake install

goto :EOF

:error
echo Failed!
EXIT /b %ERRORLEVEL%
