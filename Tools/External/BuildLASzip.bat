@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"

set CONFIG=%1
set PLATFORM=%2
set TARGET=%3

set BUILD_DIR=LASzip\%PLATFORM%

if not defined CONFIG (
    set CONFIG=Debug
)

if not defined PLATFORM (
    set PLATFORM=Win32
)

if not defined TARGET (
    set TARGET=Rebuild
)

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

if not exist %BUILD_DIR%\laszip.sln (
    cd %BUILD_DIR%
    if %PLATFORM% == Win32 (
        cmake.exe -G "Visual Studio 12 2013" ..\
    ) else (
        cmake.exe -G "Visual Studio 12 2013 Win64" ..\
    )
    cd ..\..\
)

%MSBUILD% %BUILD_DIR%\laszip.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

cd /d %CURDIR%

exit /b 0

:error
cd /d %CURDIR%
echo "Error====="
pause
exist /b 1