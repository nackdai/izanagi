@echo off

set CURDIR=%CD%

cd /d %~dp0

for /f "tokens=*" %%a in ('dir /b ..\tools\Release') do (
	if %%~xa==.exe (
		xcopy /y ..\tools\Release\%%a .\
	) else if %%~xa==.dll (
		xcopy /y ..\tools\Release\%%a .\
	)
)

set Data=.\data

if not exist %Data% (
	mkdir %Data%
)

set MediaDir=..\..\Media
set SampleDir=..\..\Samples
set LibDir=..\..\Libs

xcopy /y %MediaDir%\Seymour.dae %Data%
xcopy /y %SampleDir%\Archive\*.xml %Data%
xcopy /y %SampleDir%\Shader\shader\* %Data%
xcopy /y %SampleDir%\PostEffect\posteffect\* %Data%
xcopy /y %MediaDir%\FontSample* %Data%
xcopy /y %MediaDir%\mikachan.ttf %Data%

xcopy /y %MediaDir%\EnvMap.xml %Data%
xcopy /y %MediaDir%\mirrormap.png %Data%
xcopy /y %MediaDir%\*_uffizi_probe.* %Data%
xcopy /y %MediaDir%\*_uffizi_cross.* %Data%

xcopy /y %MediaDir%\HeightMap.png %Data%

xcopy /y %Lib

cd /d %CURDIR%