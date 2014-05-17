
#
# Qt qmake integration with Google Protocol Buffers compiler protoc
#
# To compile protocol buffers with qt qmake, specify PROTOS variable and
# include this file
#
# Example:
# LIBS += /usr/local/lib/libprotobuf.so
# PROTOS = a.proto b.proto
# include(protobuf.pri)
#
# By default protoc looks for .proto files (including the imported ones) in
# the current directory where protoc is run. If you need to include additional
# paths specify the PROTOPATH variable    

isEmpty(PROTOBUF_PROTOC): PROTOBUF_PROTOC = protoc

#PROTOSCRIPTPATH = $$PWD/../3rdparty/scripts

PROTOGEN = generated

PROTOPATH += .
PROTOPATHS =
for(p, PROTOPATH):PROTOPATHS += --proto_path=$${p}

PROTODEPPATHS =
for (p, PROTOPATH):PROTODEPPATHS += --vpath=$${p}

message("Generating protocol buffer classes from .proto files.")

#protobuf_cp.name = protobuf files
#protobuf_cp.input = PROTOS_IN
#protobuf_cp.output = $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.proto
#protobuf_cp.commands = cp ${QMAKE_FILE_NAME} $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.proto
#protobuf_cp.variable_out = PROTOS
#QMAKE_EXTRA_COMPILERS += protobuf_cp

protobuf_decl.name = protobuf headers
#protobuf_decl.input = PROTOS
protobuf_decl.input = PROTOS_IN
protobuf_decl.output = $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.pb.h
#protobuf_decl.depends = $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.proto
#protobuf_decl.dependcy_type = TYPE_C
#protobuf_decl.depend_command = python $$PROTOSCRIPTPATH/protoc-gen-depends-wrapper.py $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.d protoc --plugin=protoc-gen-depends=$$PROTOSCRIPTPATH/protoc-gen-depends --proto_path=${QMAKE_FILE_IN_PATH} $$PROTOPATHS --depends_out=\"--cgen=$$OUT_PWD/$$PROTOGEN $$PROTODEPPATHS\":$$OUT_PWD/$$PROTOGEN/ ${QMAKE_FILE_NAME}
protobuf_decl.commands = $$PROTOBUF_PROTOC --cpp_out=$$OUT_PWD/$$PROTOGEN/ --proto_path=${QMAKE_FILE_IN_PATH} $$PROTOPATHS ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = PROTOS_IN
protobuf_impl.output = $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl

INCLUDEPATH += $$OUT_PWD/$$PROTOGEN

!isEmpty(PROTOBUF_LIB_PATH): LIBS += -L$$PROTOBUF_LIB_PATH
!isEmpty(PROTOBUF_INCLUDE_PATH): INCLUDEPATH += $$PROTOBUF_INCLUDE_PATH

LIBS += -Bstatic -lprotobuf
