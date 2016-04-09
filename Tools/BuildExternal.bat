@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"
set TARGET=Rebuild
set CONFIG=%1

if not exist "External\squish\squish.sln" (
    cd External\squish
    ..\..\..\External\cmake\bin\cmake.exe -G "Visual Studio 12 2013"
    cd ..\..
)

if not exist "External\assimp\Assimp.sln" (
    cd External\assimp
    ..\..\..\External\cmake\bin\cmake.exe -G "Visual Studio 12 2013"
    cd ..\..
)

%MSBUILD% External\Preproc\Preproc.sln /t:%TARGET% /p:Configuration=%CONFIG% || goto error
%MSBUILD% External\NvTriStrip\src\NvTriStrip\NvTriStrip.sln /t:%TARGET% /p:Configuration=%CONFIG% || goto error
%MSBUILD% External\xerces\projects\Win32\VC12\xerces-all\xerces-all.sln /t:XercesLib:%TARGET% /p:Configuration=%CONFIG% || goto error
%MSBUILD% External\squish\squish.sln /t:squish:%TARGET% /p:Configuration=%CONFIG% || goto error
%MSBUILD% External\assimp\Assimp.sln /t:assimp:%TARGET% /p:Configuration=%CONFIG% || goto error

cd External
call BuildMojoShader.bat %CONFIG%
cd ..

%MSBUILD% ..\External\freetype\builds\windows\vc2013\freetype.sln /t:%TARGET% /p:Configuration=%CONFIG% || goto error

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
exit /b 0
