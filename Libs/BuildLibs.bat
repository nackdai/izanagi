set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build
set CONFIG=%1
set GFX=%2

if GFX==GLES2 (
    %MSBUILD% ..\External\ANGLE\src\ANGLE.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error
)

%MSBUILD% project\vs2010\izanagi.sln /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% || goto error

@echo off
exit /b 1

:error
@echo off
echo "Error====="
pause
