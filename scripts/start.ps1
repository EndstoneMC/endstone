$host.UI.RawUI.WindowTitle = "Endstone server for Minecraft: Bedrock Edition"

$PYTHON_BINARY = ""

if (Test-Path "bin\python\python.exe") {
    $PYTHON_BINARY = "bin\python\python.exe"
}

if (-Not $PYTHON_BINARY) {
    Write-Host "Couldn't find a Python binary in `"$($PWD.Path)\bin\python`""
    Write-Host "Please refer to the installation instructions at https://endstone.readthedocs.io/en/latest/getting-started/installation"
    Write-Host ""
    Pause
    Exit 1
}

& $PYTHON_BINARY -m endstone $args
if ($LASTEXITCODE -ne 0) { Pause }
