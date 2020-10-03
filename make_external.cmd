@echo off
rmdir /S /Q generated
mkdir generated
for %%f in (*.proto) do (
    echo %%~nf
	..\CKSimLibCpp\lib\protobuf\protoc.exe %%~nf.proto --cpp_out=generated
)
cd generated
mkdir include
mkdir src
for %%f in (*.cc) do (
	move %%~nf.cc src/ > nul
)
for %%f in (*.h) do (
	move %%~nf.h include/ > nul
)
cd ..
echo Generated!
