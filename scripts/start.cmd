@echo off
TITLE Endstone server for Minecraft: Bedrock Edition

set PYTHON_BINARY=

if exist bin\python\python.exe (
	set PYTHON_BINARY=bin\python\python.exe
)

if "%PYTHON_BINARY%"=="" (
	echo Couldn't find a Python binary in "%~dp0bin\python"
	echo Please refer to the installation instructions at https://endstone.readthedocs.io/en/latest/getting-started/installation
	echo:
	pause
	exit 1
)

%PYTHON_BINARY% -m endstone %* || pause
