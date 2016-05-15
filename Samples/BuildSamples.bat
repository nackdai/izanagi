@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"
set TARGET=Build
set CONFIG=%1
set GFX=%2

%MSBUILD% projects\vs2013\Samples.sln /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% || goto error

cd /d %CURDIR%

exit /b 0

:error
cd /d %CURDIR%
echo "Error====="
pause
exit /b 1
