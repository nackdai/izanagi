set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build
set CONFIG=%1

%MSBUILD% ..\Libs\project\vs2010\Math.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_DX9 || goto error
%MSBUILD% projects\vs2010\Tools.sln /t:%TARGET% /p:Configuration=%CONFIG% || goto error

@echo off
exit /b 1

:error
@echo off
echo "Error====="
pause
