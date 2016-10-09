setlocal
@echo ON

mkdir -p tmp
cd tmp

appveyor DownloadFile http://buildbot.roessler.systems/files/qt-bin/protobuf-win64.7z -Filename protolibs.7z
7z x protolibs.7z
cd protolibs
SET PROTODIR=%HOMEDRIVE%%HOMEPATH%\bin\protobuf\vsprojects\x64\Release
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

appveyor DownloadFile http://buildbot.roessler.systems/files/qt-bin/zeromq-win64.7z -Filename zmqlibs.7z
7z x zmqlibs.7z
cd zmqlibs
SET ZEROMQDIR=%HOMEDRIVE%%HOMEPATH%\bin\zeromq4-x\lib\x64
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
cp -r %QTDIR%/qml/Machinekit qml/
7z a QtQuickVcp.zip qml/

goto :EOF

:error
echo Failed!
EXIT /b %ERRORLEVEL%
