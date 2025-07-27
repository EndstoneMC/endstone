#!/bin/bash

# Made by @legitbox, this creature will auto install the endstone server software depending on what server os you are using


# Define the virtual environment directory
VENV_DIR="$HOME/.virtualenvs/endstone"

# ASCII Art
echo -e "\e[0;33m"
echo "                       888          888                              "
echo "                       888          888                              "
echo "                       888          888                              "
echo " .d88b.  88888b.   .d88888 .d8888b  888888 .d88b.  88888b.   .d88b.  "
echo "d8P  Y8b 888 \"88b d88\" 888 88K      888   d88\"\"88b 888 \"88b d8P  Y8b "
echo "88888888 888  888 888  888 \"Y8888b. 888   888  888 888  888 88888888 "
echo "Y8b.     888  888 Y88b 888      X88 Y88b. Y88..88P 888  888 Y8b.     "
echo " \"Y8888  888  888  \"Y88888  88888P'  \"Y888 \"Y88P\"  888  888  \"Y8888  "
echo "                                                                     "
echo "                                                                     "
echo -e "\e[0m"

# Display a message and countdown
echo "The installer will start in 5 seconds."
echo "You can stop it by pressing Ctrl+C."
sleep 5

# Function to install and setup on Debian/Ubuntu
setup_debian() {
    sudo apt-get update
    sudo apt-get install -y python3 python3-venv
}

# Function to install and setup on Fedora
setup_fedora() {
    sudo dnf install -y python3 python3-virtualenv
}

# Function to install and setup on Arch
setup_arch() {
    sudo pacman -Syu --noconfirm
    sudo pacman -S --noconfirm python python-virtualenv
}

# Detect the Linux distribution
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS_FAMILY="${ID_LIKE:-$ID}"  # Use ID_LIKE if available, otherwise use ID
    case $OS_FAMILY in
        *debian*|*ubuntu*)
            echo "Detected Debian/Ubuntu based system."
            setup_debian
            ;;
        *fedora*)
            echo "Detected Fedora based system."
            setup_fedora
            ;;
        *arch*)
            echo "Detected Arch based system."
            setup_arch
            ;;
        *)
            echo "Unsupported Linux distribution: $OS_FAMILY"
            exit 1
            ;;
    esac
else
    echo "Cannot detect the Linux distribution."
    exit 1
fi

# Create the virtual environment if it doesn't exist
if [ ! -d "$VENV_DIR" ]; then
    python3 -m venv "$VENV_DIR"
fi

# Activate the virtual environment
source "$VENV_DIR/bin/activate"

# Upgrade pip to the latest version
pip install --upgrade pip

# Install the 'endstone' package
pip install endstone

# Run 'endstone'
endstone
