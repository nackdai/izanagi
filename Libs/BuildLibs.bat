rem @echo off

set CURDIR=%CD%

cd /d %~dp0

if not defined DXSDK_DIR (
    echo "There is no DX SDK"
    goto error
)

call ..\Build\SetBuildParams.bat

set TARGET=Build
set CONFIG=%1
set GFX=%2

set BUILD=%3

if not defined BUILD (
    set BUILD=All
)

set TOOL_CONFIG=Release

rem ShaderCompiler ================================
call ..\Tools\External\BuildMojoShader.bat %TOOL_CONFIG%
%MSBUILD% ..\Tools\External\Preproc\Preproc.sln /t:%TARGET% /p:Configuration=%TOOL_CONFIG% || goto error
%MSBUILD% ..\Tools\projects\vs2013\ShaderCompiler.sln /t:%TARGET% /p:Configuration=%TOOL_CONFIG% || goto error

if %GFX%==GLES2 (
    %MSBUILD% ..\External\ANGLE\projects\src\ANGLE.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error
)

if %GFX%==OGL (
    call ..\External\BuildExternalOGL.bat %CONFIG%
)

if %BUILD%==All (
    call ..\External\BuildExternal.bat %CONFIG%
    %MSBUILD% project\vs2013\izanagi.sln /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error
) else (
    %MSBUILD% project\vs2013\Std.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error
    %MSBUILD% project\vs2013\System.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error
    %MSBUILD% project\vs2013\Math.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error
    %MSBUILD% project\vs2013\Graph.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error
    %MSBUILD% project\vs2013\SceneGraph.vcxproj /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% /p:Platform=Win32 || goto error
)

cd /d %CURDIR%

exit /b 0

:error
cd /d %CURDIR%
echo "Error====="
pause
exist /b 1