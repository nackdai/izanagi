@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build
set CONFIG=Debug

%MSBUILD% .\projects\vs2010\Tests.sln /t:%TARGET% /p:Configuration=%CONFIG% || goto error

for /f "tokens=*" %%a in ('dir /b .\tests\%CONFIG%\*.exe') do (
	RunTest.bat %%~na
)

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
