set CURDIR=%CD%

cd /d %~dp0

cd depot_tools

gclient

set GYP_GENERATORS=msvs
rem set GYP_DEFINES="windows_sdk_path=YOUR_WIN_SDK_INSTALL_DIR"
set GYP_MSVS_VERSION=2013

cd ..\ANGLE

python scripts/bootstrap.py
gclient sync

cd /d %CURDIR%
