echo off

if not defined MSBUILD (
    set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
)

if not defined TARGET (
    set TARGET=Rebuild
)

if not defined CONFIG (
    set CONFIG=%1
)

set SOLUTION=".\mojoshader\MojoShader.sln"

if not exist %SOLUTION% (
    cd mojoshader
    ..\..\..\External\cmake\bin\cmake.exe -G "Visual Studio 10"
    cd ..
)

%MSBUILD% %SOLUTION% /t:%TARGET% /p:Configuration=%CONFIG% || goto error

@echo off
exit /b 1
