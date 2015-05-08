@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build
set CONFIG=%1
set GFX=%2

%MSBUILD% projects\vs2010\Samples.sln /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% || goto error

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
