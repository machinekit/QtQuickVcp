setlocal

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
