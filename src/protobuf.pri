
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

PROTOGEN = generated

PROTOPATH += .
PROTOPATHS =
for(p, PROTOPATH):PROTOPATHS += --proto_path=$${p}

PROTODEPPATHS =
for (p, PROTOPATH):PROTODEPPATHS += --vpath=$${p}

message("Generating protocol buffer classes from .proto files.")

protobuf_decl.name = protobuf headers
protobuf_decl.input = PROTOS_IN
protobuf_decl.output = $$OUT_PWD/$$PROTOGEN/${QMAKE_FILE_BASE}.pb.h
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

!isEmpty(PROTOBUF_INCLUDE_PATH): INCLUDEPATH += $$PROTOBUF_INCLUDE_PATH
!isEmpty(PROTOBUF_LIB_PATH): LIBS += -L$$PROTOBUF_LIB_PATH
!isEmpty(PROTOBUF_LIB_FLAGS): LIBS += $$PROTOBUF_LIB_FLAGS
LIBS += -lprotobuf
