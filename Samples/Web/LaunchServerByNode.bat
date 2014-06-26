@echo off

for /F %%t "usebackq" in (`where node.exe') do set NODE_ROOT=%%t

if ""%NODE_ROOT""=="""" (
	echo "You have to install Node.js."
	exit /b 1
)

for /F %%t "usebackq" in (`where npm') do set NPM_ROOT=%%t

if ""%NPM_ROOT""=="""" (
	echo "You have to install npm."
	exit /b 1
)

npm install
node.exe server.js
