set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build

%MSBUILD% projects\vs2010\Samples.sln /t:%TARGET% /p:Configuration=Release_DX9 || goto error

@echo off
exit /b 1

:error
@echo off
echo "Error====="
pause
