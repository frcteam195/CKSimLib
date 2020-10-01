### CKSimLib

To generate proto cpp source, clone CKSimProtoSpec. protoc each file as such:
mkdir generated
..\CKSimLibCpp\lib\protobuf\protoc.exe ValueMessage.proto --cpp_out=generated

Copy the generated source files to the protobuf folder in src
Will work on adding an automated task for this later