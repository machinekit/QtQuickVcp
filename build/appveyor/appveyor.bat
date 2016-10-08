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

appveyor DownloadFile https://github.com/google/protobuf/archive/2.7.0.zip -Filename protosrc.zip
7z x protosrc.zip

cd protobuf-2.7.0
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
cd ..

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
