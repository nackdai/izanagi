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

rem freetype =========================

%MSBUILD% freetype\builds\windows\vc2013\freetype.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error

rem libuv =============================

if not exist libuv\uv.sln (
    call libuv\vcbuild.bat
) else (
    %MSBUILD% libuv\libuv.vcxproj /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% || goto error
)

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
