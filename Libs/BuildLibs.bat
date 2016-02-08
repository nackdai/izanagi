@echo off

set CURDIR=%CD%

cd /d %~dp0

set MSBUILD="C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"
set TARGET=Build
set CONFIG=%1
set GFX=%2

if %GFX%==GLES2 (
    %MSBUILD% ..\External\ANGLE\projects\src\ANGLE.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error

    cd ..\Tools
    cd External
    call BuildMojoShader.bat Release
    cd ..
    %MSBUILD% External\Preproc\Preproc.sln /t:%TARGET% /p:Configuration=Release || goto error
    %MSBUILD% projects\vs2013\ShaderCompiler.sln /t:%TARGET% /p:Configuration=Release || goto error

    cd ..\Libs
)

if %GFX%==OGL (
    %MSBUILD% ..\External\glew\glew.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error
    %MSBUILD% ..\External\freeglut\freeglut\freeglut\freeglut.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error

    cd ..\Tools
    cd External
    call BuildMojoShader.bat Release
    cd ..
    %MSBUILD% External\Preproc\Preproc.sln /t:%TARGET% /p:Configuration=Release || goto error
    %MSBUILD% projects\vs2013\ShaderCompiler.sln /t:%TARGET% /p:Configuration=Release || goto error

    cd ..\Libs

    set GFX=OGL
)

%MSBUILD% ..\External\freetype\builds\freetype.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error
%MSBUILD% ..\External\libuv\uv.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error

%MSBUILD% project\vs2013\izanagi.sln /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error

set CURDIR=%CD%

exit /b 1

:error
set CURDIR=%CD%
echo "Error====="
pause
