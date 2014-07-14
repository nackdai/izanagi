@echo off
for /f "tokens=*" %%a in ('dir /b ..\*') do if not %%a==Bin (if not exist .\%%a (mkdir .\%%a))

for /f "tokens=*" %%a in ('dir /b ..\*') do for /f "tokens=*" %%b in ('dir /b/s ..\%%a\*.exe') do if not %%a==Bin (xcopy /y %%b .\%%a)

for /f "tokens=*" %%a in ('dir /b ..\*') do if not %%a==Bin (if not exist .\%%a\data (mkdir .\%%a\data))

for /f "tokens=*" %%a in ('dir /b ..\*') do if not %%a==Bin (if exist ..\%%a\data (xcopy /y/e ..\%%a\data\* .\%%a\data\))

if exist .\SampleRunner\SampleRunner.exe (
	xcopy /y .\SampleRunner\SampleRunner.exe .\
	rd /s/q .\SampleRunner
)
