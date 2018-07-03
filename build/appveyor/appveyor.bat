setlocal
@echo ON

cd %APPVEYOR_BUILD_FOLDER%
mkdir -p tmp
cd tmp

:: get version label
appveyor DownloadFile http://ci.roessler.systems/files/qt-bin/UnxUtils.zip -Filename UnxUtils.zip
7z x UnxUtils.zip || goto :error
cp usr\local\wbin\date.exe . || goto :error

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
echo #define REVISION "%version%" > src\application\revision.h || goto :error
appveyor UpdateBuild -Version "%version%-%ARCH%" || goto :error


cd tmp
appveyor DownloadFile http://ci.roessler.systems/files/qt-bin/protobuf-win-%ARCH%.7z -Filename protolibs.7z || goto :error
7z x protolibs.7z || goto :error
cd protolibs
if %ARCH% == x64 (
    SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\vsprojects\x64\Release
) else (
    SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\vsprojects\Release
)
mkdir -p %PROTODIR%
mv protoc.exe %PROTODIR%\ || goto :error
mv libprotoc.lib %PROTODIR%\ || goto :error
cp libprotobuf.lib %PROTODIR%\ || goto :error
mv libprotobuf.lib %QTDIR%\lib\ || goto :error
cd ..

SET PROTOVERSION=2.6.1
appveyor DownloadFile https://github.com/google/protobuf/archive/v%PROTOVERSION%.zip -Filename protosrc.zip
7z x protosrc.zip || goto :error
cd protobuf-%PROTOVERSION% || goto :error
SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\
cp -r src %PROTODIR% || goto :error
cd .. || goto :error

appveyor DownloadFile http://ci.roessler.systems/files/qt-bin/zeromq-win-%ARCH%.7z -Filename zmqlibs.7z || goto :error
7z x zmqlibs.7z || goto :error
cd zmqlibs
if %ARCH% == x64 (
    SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x\lib\x64
) else (
    SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x\lib\Win32
)
mkdir -p %ZEROMQDIR%
cp libzmq.lib %ZEROMQDIR%\ || goto :error
mv libzmq.lib %QTDIR%\lib\ || goto :error
mv libzmq.dll %QTDIR%\bin\ || goto :error
cd ..

SET ZMQVERSION=4.0.8
appveyor DownloadFile https://github.com/zeromq/zeromq4-x/archive/v%ZMQVERSION%.zip -Filename zmqsrc.zip || goto :error
7z x zmqsrc.zip || goto :error
cd zeromq4-x-%ZMQVERSION%
SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x
cp -r include %ZEROMQDIR% || goto :error
cd ..

:: start build
cd %APPVEYOR_BUILD_FOLDER%
mkdir build.release
cd build.release
qmake -r .. || goto :error
nmake || goto :error
nmake install || goto :error

mkdir MachinekitClient
cd MachinekitClient
cp ../apps/MachinekitClient/release/machinekit-client.exe . || goto :error
windeployqt --angle --release --qmldir ../../apps/MachinekitClient/ machinekit-client.exe || goto :error
cp ../translations/*.qm translations/ || goto :error
cp %QTDIR%\bin\libzmq.dll . || goto :error
cd .. || goto :error
7z a MachinekitClient.zip MachinekitClient/ || goto :error

mkdir qml
mkdir lib
cp -r %QTDIR%/qml/Machinekit qml/ || goto :error
cp -r %QTDIR%/bin/libzmq.dll lib/ || goto :error
7z a QtQuickVcp.zip qml/ lib/ translations/ || goto :error

:: rename deployment files
set platform=%ARCH%
if %release% == 0 (
    set target1="QtQuickVcp_Development"
    set target2="MachinekitClient_Development"
) else (
    set target1="QtQuickVcp"
    set target2="MachinekitClient"
)

mv QtQuickVcp.zip %target1%-%version%-Windows-%platform%.zip || goto :error
mv MachinekitClient.zip %target2%-%version%-%platform%.zip || goto :error

goto :EOF

:error
echo Failed!
EXIT /b %ERRORLEVEL%
