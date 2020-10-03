@echo off
set "rootDir=%cd%\..\"
cd "%rootDir%\src\main\driver\include"
rmdir /S /Q protobuf
mkdir protobuf
cd "%rootDir%\src\main\driver\cpp"
rmdir /S /Q protobuf
mkdir protobuf
cd "%rootDir%"
rmdir /S /Q generated
mkdir generated
cd external\CKSimProtoSpec
for %%f in (*.proto) do (
    echo %%~nf
	"%rootDir%\lib\protobuf\protoc.exe" %%~nf.proto --cpp_out="%rootDir%\generated"
)
cd "%rootDir%\generated"
for %%f in (*.cc) do (
	move %%~nf.cc "%rootDir%\src\main\driver\cpp\protobuf\" > nul
)
for %%f in (*.h) do (
	move %%~nf.h "%rootDir%\src\main\driver\include\protobuf\" > nul
)
cd "%rootDir%"
echo Generated!
