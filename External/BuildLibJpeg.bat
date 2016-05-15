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

set BUILD_DIR=libjpeg\%PLATFORM%

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

set CMAKE_OPTIONS=-DWITH_SIMD=FALSE -DENABLE_SHARED=FALSE

if not exist %BUILD_DIR%\libjpeg-turbo.sln (
    cd %BUILD_DIR%
    if %PLATFORM% == Win32 (
        ..\..\cmake\bin\cmake.exe -G "Visual Studio 12 2013" %CMAKE_OPTIONS% ..\
    ) else (
        ..\..\cmake\bin\cmake.exe -G "Visual Studio 12 2013 Win64" %CMAKE_OPTIONS% ..\
    )
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