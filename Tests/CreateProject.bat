@echo off

set CURDIR=%CD%

cd /d %~dp0

set EXE_PATH="..\Tools\tools\Release\CopyVCProject.exe"
set PRJ_PATH=".\projects\vs2010"

set SRC=%1
set DST=%2

set EXT=".vcxproj"

if exist %EXE_PATH% (
	%EXE_PATH% %PRJ_PATH%\%SRC%%EXT% %PRJ_PATH%\%DST%%EXT%
) else (
	echo "Not exist CopyVCProject.exe"
	goto error
)

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
