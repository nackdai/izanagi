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

set BUILD_DIR=freeglut\%PLATFORM%

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

if not exist %BUILD_DIR%\freeglut.sln (
    cd %BUILD_DIR%
    if %PLATFORM% == Win32 (
        ..\..\cmake\bin\cmake.exe -G "Visual Studio 14 2015" ..\
    ) else (
        ..\..\cmake\bin\cmake.exe -G "Visual Studio 14 2015 Win64" ..\
    )
    cd ..\..\
)

%MSBUILD% %BUILD_DIR%\freeglut.vcxproj /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

rem glew =============================

%MSBUILD% glew\build\vc12\glew.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

rem glfw =============================

set BUILD_DIR=glfw\%PLATFORM%

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

if not exist %BUILD_DIR%\GLFW.sln (
    cd %BUILD_DIR%
    if %PLATFORM% == Win32 (
        ..\..\cmake\bin\cmake.exe -D USE_MSVC_RUNTIME_LIBRARY_DLL=OFF -G "Visual Studio 14 2015" ..\
    ) else (
        ..\..\cmake\bin\cmake.exe -D USE_MSVC_RUNTIME_LIBRARY_DLL=OFF -G "Visual Studio 14 2015 Win64" ..\
    )
    cd ..\..\
)

%MSBUILD% %BUILD_DIR%\GLFW.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

rem end ==============================

cd /d %CURDIR%

exit /b 0

:error
cd /d %CURDIR%
echo "Error====="
pause
exist /b 1