REM @echo off
cd ..
set "rootDir=%cd%\"
cd "%rootdir%..\CKMavenRepo\com\team195\"
set "mavenPubDir=%cd%\"
xcopy "%rootDir%build\repos\releases\com\team195\CKSim-cpp" "%mavenPubDir%CKSim-cpp\" /Y /E
xcopy "%rootDir%build\repos\releases\com\team195\CKSim-java" "%mavenPubDir%CKSim-java\" /Y /E
xcopy "%rootDir%build\repos\releases\com\team195\CKSim-driver" "%mavenPubDir%CKSim-driver\" /Y /E
