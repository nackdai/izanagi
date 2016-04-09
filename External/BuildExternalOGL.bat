@echo off

set CURDIR=%CD%

cd /d %~dp0

call ..\Build\SetBuildParams.bat

set TARGET=Build
set CONFIG=%1
set PLATFORM=%2

if not defined CONFIG (
    set CONFIG=Debug
)

if not defined PLATFORM (
    set PLATFORM=Win32
)

rem freeglut =========================

if not exist freeglut\freeglut.sln (
    cd freeglut
    ..\cmake\bin\cmake.exe -G "Visual Studio 12 2013"
    cd ..
)

%MSBUILD% freeglut\freeglut.vcxproj /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

rem glew =============================

%MSBUILD% glew\build\vc12\glew.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
exist /b 0