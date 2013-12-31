set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build
set CONFIG=%1
set GFX=%2

if %GFX%==GLES2 (
    %MSBUILD% ..\External\ANGLE\src\ANGLE.sln /t:%TARGET% /p:Configuration=%CONFIG% /p:Platform=Win32 || goto error

    cd ..\Tools
    cd External
    call BuildMojoShader.bat Release
    cd ..
    %MSBUILD% External\Preproc\Preproc_VC10.sln /t:%TARGET% /p:Configuration=Release || goto error
    %MSBUILD% projects\vs2010\ShaderCompiler.sln /t:%TARGET% /p:Configuration=Release || goto error

    cd ..\Libs
)

%MSBUILD% ..\External\freetype\builds\win32\vc2010\freetype.sln /t:%TARGET% /p:Configuration="%CONFIG% Multithreaded" || goto error

%MSBUILD% project\vs2010\izanagi.sln /t:%TARGET% /p:Configuration=%CONFIG%_%GFX% || goto error

@echo off
exit /b 1

:error
@echo off
echo "Error====="
pause
