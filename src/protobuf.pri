
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
#

PROTOPATH += .
PROTOPATH += ../Protocol
PROTOPATHS =
for(p, PROTOPATH):PROTOPATHS += --proto_path=$${p}

message("Generating protocol buffer classes from .proto files.")

protobuf_cp.name = protobuf files
protobuf_cp.input = PROTOS_IN
protobuf_cp.output = $$PWD/${QMAKE_FILE_BASE}.proto
protobuf_cp.commands = cp ${QMAKE_FILE_NAME} $$PWD/${QMAKE_FILE_BASE}.proto
protobuf_cp.variable_out = PROTOS
QMAKE_EXTRA_COMPILERS += protobuf_cp

protobuf_decl.name = protobuf headers
protobuf_decl.input = PROTOS
protobuf_decl.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.pb.h
protobuf_decl.depends = $$PWD/${QMAKE_FILE_BASE}.proto
protobuf_decl.commands = protoc --cpp_out=${QMAKE_FILE_IN_PATH} --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = PROTOS
protobuf_impl.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl
