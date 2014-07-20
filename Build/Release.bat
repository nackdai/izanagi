@echo off

if exist ..\Release (
	rmdir /s/q ..\Release
)

if not exist ..\Release (
	mkdir ..\Release
)

xcopy /E/Y/D/I ..\Build ..\Release\Build
xcopy /E/Y/D/I ..\External ..\Release\External
xcopy /E/Y/D/I ..\Libs ..\Release\Libs
xcopy /E/Y/D/I ..\Media ..\Release\Media
xcopy /E/Y/D/I ..\Tools ..\Release\Tools

rmdir /s/q ..\Release\External\emscripten
rmdir /s/q ..\Release\Tools\External\collada-dom
