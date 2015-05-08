@echo off

set CURDIR=%CD%

cd /d %~dp0

cd ..\

rmdir /s/q External\emscripten
rmdir /s/q Tools\External\collada-dom

cd Build

call BuildAll_Release_DX9.bat

cd ..\Samples\Bin

call Gather.bat

cd ..\Tools\Bin

call Gather.bat

cd /d %CURDIR%
