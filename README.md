# Endstone

Endstone provides a Python-based plugin API, allowing developers to enhance Bedrock Dedicated Servers with various
features and functionalities. This project requires Python 3.9 or higher.

## Installing Python

Before you start with Endstone, make sure you have Python 3.9 or higher installed on your system. Here's how you can
install it:

### Windows

1. Visit the [Python Downloads page](https://www.python.org/downloads/) for Windows.
2. Download the installer for Python 3.9 or higher.
3. Run the installer and follow the on-screen instructions, making sure to check the box that adds Python to your PATH.

### Linux

The method of installing Python varies between Linux distributions. Here's a general guide for Ubuntu:

1. Open a terminal window.
2. Run the following command to update your package list:

   ```bash
   sudo apt update
   ```

3. Run the following command to install Python 3.9:

   ```bash
   sudo apt install python3.9
   ```

## Quick Start

Here's how you can get started with Endstone:

1. **Create a virtual environment**

   In your terminal, navigate to the directory where you want to create your virtual environment and run:

   ```bash
   python -m venv venv
   ```

   This creates a new virtual environment in a folder named `venv`.

2. **Activate the virtual environment**

   On Windows:

   ```bash
   .\venv\Scripts\activate
   ```

   On Linux:

   ```bash
   source venv/bin/activate
   ```

3. **Install Endstone**

   You have two options to install Endstone:

    - **Option A: Install from PyPI**

      Run the following command in your terminal to install Endstone from PyPI:

      ```bash
      pip install endstone
      ```

    - **Option B: Install locally**

      You can also clone the repository and install it locally. First, clone the repository:

      ```bash
      git clone https://github.com/pybedrock/endstone.git
      ```

      Navigate into the `endstone` directory:

      ```bash
      cd endstone
      ```

      Then install locally:

      ```bash
      pip install -U .
      ```

4. **Start the server**

   Run the following command to start the server:

   ```bash
   endstone
   ```

Congratulations! You're now ready to use Endstone to create plugins for Bedrock Dedicated Servers.