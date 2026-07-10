@echo off
TITLE Endstone server for Minecraft: Bedrock Edition

setlocal

pushd "%~dp0"

set "WHEEL="
for %%f in ("%~dp0endstone-*.whl") do set "WHEEL=%%f"

if "%WHEEL%"=="" (
    echo Couldn't find an endstone wheel next to "%~dp0start.cmd".
    echo Please re-extract the bundle archive without removing the .whl file.
    echo:
    pause
    popd
    exit /b 1
)

where uv >nul 2>&1
if errorlevel 1 (
    if exist "%USERPROFILE%\.local\bin\uv.exe" (
        set "PATH=%USERPROFILE%\.local\bin;%PATH%"
    ) else (
        echo Installing uv...
        powershell -ExecutionPolicy ByPass -NoProfile -Command "irm https://astral.sh/uv/install.ps1 | iex"
        if errorlevel 1 (
            echo Failed to install uv. See https://docs.astral.sh/uv/getting-started/installation/ for manual installation.
            pause
            popd
            exit /b 1
        )
        set "PATH=%USERPROFILE%\.local\bin;%PATH%"
    )
)

if not exist ".venv\Scripts\python.exe" (
    echo Creating virtual environment...
    uv venv --managed-python --python 3.13 .venv
    if errorlevel 1 (
        echo Failed to create the virtual environment.
        pause
        popd
        exit /b 1
    )
)

uv pip install --python .venv "%WHEEL%"
if errorlevel 1 (
    echo Failed to install "%WHEEL%".
    pause
    popd
    exit /b 1
)

".venv\Scripts\python.exe" -m endstone -i %* || pause

popd
endlocal
