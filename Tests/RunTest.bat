@echo off

if not defined CONFIG (
    set CONFIG=Debug
)

if not defined CURDIR (
	set CURDIR=%CD%
)

cd /d %~dp0

set TEST=%1

.\tests\%CONFIG%\%TEST%.exe || goto error

cd /d %CURDIR%

exit /b 1

:error
cd /d %CURDIR%
echo "Error====="
pause
