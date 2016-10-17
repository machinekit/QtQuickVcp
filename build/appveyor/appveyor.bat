setlocal
@echo ON

cd %APPVEYOR_BUILD_FOLDER%
mkdir -p tmp
cd tmp

:: get version label
appveyor DownloadFile http://buildbot.roessler.systems/files/qt-bin/UnxUtils.zip -Filename UnxUtils.zip
7z x UnxUtils.zip
cp usr\local\wbin\date.exe .

cd ..

git describe --exact-match HEAD
if %ERRORLEVEL% == 0 (
    SET release=1
) else (
    SET release=0
)
if %release% == 0 (
    for /f %%i in ('tmp\date.exe -u +"%%Y%%m%%d%%H%%M"') do set datetime=%%i

    ::for /f %%i in ('git rev-parse --abbrev-ref HEAD') do set branch=%%i
    set branch=%APPVEYOR_REPO_BRANCH%

    for /f %%i in ('git rev-parse --short HEAD') do set revision=%%i
) else (
    for /f %%i in ('git describe --tags') do set version=%%i
)

if %release% == 0 (
    set version=%datetime%-%branch%-%revision%
)
echo #define REVISION "%version%" > src\application\revision.h
appveyor UpdateBuild -Version "%version%-%ARCH%"


cd tmp
appveyor DownloadFile http://buildbot.roessler.systems/files/qt-bin/protobuf-win-%ARCH%.7z -Filename protolibs.7z
7z x protolibs.7z
cd protolibs
if %ARCH% == x64 (
    SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\vsprojects\x64\Release
) else (
    SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\vsprojects\Release
)
mkdir -p %PROTODIR%
mv protoc.exe %PROTODIR%\
mv libprotoc.lib %PROTODIR%\
cp libprotobuf.lib %PROTODIR%\
mv libprotobuf.lib %QTDIR%\lib\
cd ..

SET PROTOVERSION=2.6.1
appveyor DownloadFile https://github.com/google/protobuf/archive/v%PROTOVERSION%.zip -Filename protosrc.zip
7z x protosrc.zip
cd protobuf-%PROTOVERSION%
SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\
cp -r src %PROTODIR%
cd ..

appveyor DownloadFile http://buildbot.roessler.systems/files/qt-bin/zeromq-win-%ARCH%.7z -Filename zmqlibs.7z
7z x zmqlibs.7z
cd zmqlibs
if %ARCH% == x64 (
    SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x\lib\x64
) else (
    SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x\lib\Win32
)
mkdir -p %ZEROMQDIR%
cp libzmq.lib %ZEROMQDIR%\
mv libzmq.lib %QTDIR%\lib\
mv libzmq.dll %QTDIR%\bin\
cd ..

SET ZMQVERSION=4.0.8
appveyor DownloadFile https://github.com/zeromq/zeromq4-x/archive/v%ZMQVERSION%.zip -Filename zmqsrc.zip
7z x zmqsrc.zip
cd zeromq4-x-%ZMQVERSION%
SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x
cp -r include %ZEROMQDIR%
cd ..

:: start build
cd %APPVEYOR_BUILD_FOLDER%
mkdir build.release
cd build.release
qmake ..
nmake
nmake install

mkdir MachinekitClient
cd MachinekitClient
cp ../apps/MachinekitClient/release/machinekit-client.exe .
windeployqt --angle --release --qmldir ../../apps/MachinekitClient/ machinekit-client.exe
cd ..
7z a MachinekitClient.zip MachinekitClient/

mkdir qml
mkdir lib
cp -r %QTDIR%/qml/Machinekit qml/
cp -r %QTDIR%/bin/libzmq.dll lib/
7z a QtQuickVcp.zip qml/ lib/

:: rename deployment files
set platform=%ARCH%
if %release% == 0 (
    set target1="QtQuickVcp_Development"
    set target2="MachinekitClient_Development"
) else (
    set target1="QtQuickVcp"
    set target2="MachinekitClient"
)

mv QtQuickVcp.zip %target1%-%version%-%platform%.zip
mv MachinekitClient.zip %target2%-%version%-%platform%.zip

goto :EOF

:error
echo Failed!
EXIT /b %ERRORLEVEL%
