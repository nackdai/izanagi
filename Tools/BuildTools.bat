@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"
set TARGET=Build
set CONFIG=%1

rem %MSBUILD% ..\Libs\project\vs2013\Math.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_DX9 || goto error
%MSBUILD% projects\vs2013\Tools.sln /t:%TARGET% /p:Configuration=%CONFIG% || goto error

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
