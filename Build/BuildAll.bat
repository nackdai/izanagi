set CONFIG=%1
set GFX=%2

set CURDIR=%CD%

cd /d %~dp0

cd ..\Tools
call BuildExternal_Release.bat
call BuildTools_%CONFIG%.bat
cd ..

cd Libs
call BuildLibs_%CONFIG%_%GFX%.bat
cd ..

cd Samples
call BuildSamples_%CONFIG%_%GFX%.bat
cd ..

cd /d %CURDIR%