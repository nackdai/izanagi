SET Emscripten=..\..\..\External\emscripten\emscripten\
SET Version=1.16.0

PATH=%PATH%;%Emscripten%%Version%

IF NOT EXIST ../lib (
    mkdir ../lib
)

for /F "usebackq"  %%t in (`where python`) do set PYTHON_ROOT=%%t

if ""%PYTHON_ROOT%""=="""" (
	set PYTHON_ROOT=..\..\..\External\emscripten\python\2.7.5.3_64bit
)

PATH=%PATH%;%PYTHON_ROOT%
