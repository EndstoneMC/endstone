#!/bin/bash

# @legitbox here! this file enters the virtual environment and starts endstone, if the software crashes or stops, it autorestarts itself

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

# Define the virtual environment directory
VENV_DIR="$HOME/venv"

# Function to run the endstone command with automatic restart on crash
run_endstone() {
    while true; do
        # Activate the virtual environment
        source "$VENV_DIR/bin/activate"

        # Run 'endstone'
        endstone

        # If the program exits, wait for 3 seconds and restart
        echo "The program crashed or stopped. Restarting in 3 seconds..."
        echo "If you want to quit, press CTRL + C"
        sleep 3
    done
}

# Start the loop to run endstone
run_endstone
