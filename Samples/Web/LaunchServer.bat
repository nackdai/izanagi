@echo off

set HOST=127.0.0.1
set PORT=8000


for /F "usebackq"  %%t in (`where python`) do set PYTHON_ROOT=%%t

@rem if ""%PYTHON_ROOT%""=="""" (
@rem 	set PYTHON_ROOT=..\..\External\emscripten\python\2.7.5.3_64bit
@rem )

@rem PATH=%PATH%;%PYTHON_ROOT%

python.exe server.py %HOST%:%PORT%
