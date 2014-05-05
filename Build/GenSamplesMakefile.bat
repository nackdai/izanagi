set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set MAKEITSO="..\External\makeitso\output\MakeItSo.exe"

set CURDIR = %CD%

cd /d %~dp0

if not exist %MAKEITSO% (
    %MSBUILD% ..\External\makeitso\MakeItSo.sln /t:Build /p:Configuration=Release || goto error
)

%MAKEITSO% -file=..\Samples\projects\vs2010\Samples.sln -config=MakeItSo_Samples.config

copy /Y ..\Samples\projects\vs2010\*.makefile ..\Samples\projects\makefile\
copy /Y ..\Samples\projects\vs2010\Makefile ..\Samples\projects\makefile\

cd /d %CURDIR%

@echo off
exit /b 1

:error
@echo off
echo "Error====="
pause
