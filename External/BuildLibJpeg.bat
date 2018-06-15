@echo off

set CURDIR=%CD%

cd /d %~dp0

call ..\Build\SetBuildParams.bat

set TARGET=Build
set CONFIG=%1
set PLATFORM=%2
set VSVER=%3

if not defined CONFIG (
    set CONFIG=Debug
)

if not defined PLATFORM (
    set PLATFORM=Win32
)

if not defined VSVER (
    set VSVER=2015
)

set VS="Visual Studio 12 2013"

if %VSVER% == 2015 (
    set VS="Visual Studio 14 2015"
    if %PLATFORM% == x64 (
        set VS="Visual Studio 14 2015 Win64"
    )
) else (
    set VS="Visual Studio 12 2013"
    if %PLATFORM% == x64 (
        set VS="Visual Studio 12 2013 Win64"
    )
)

set BUILD_DIR=libjpeg\%PLATFORM%

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

set CMAKE_OPTIONS=-DWITH_SIMD=FALSE -DENABLE_SHARED=FALSE

if not exist %BUILD_DIR%\libjpeg-turbo.sln (
    cd %BUILD_DIR%

    ..\..\cmake\bin\cmake.exe -G %VS% %CMAKE_OPTIONS% ..\

    cd ..\..\
)

%MSBUILD% %BUILD_DIR%\jpeg-static.vcxproj /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error


cd /d %CURDIR%

exit /b 0

:error
cd /d %CURDIR%
echo "Error====="
pause
exist /b 1