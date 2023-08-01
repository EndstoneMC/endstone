# Endstone

Endstone provides a Python-based plugin API, allowing developers to enhance Bedrock Dedicated Servers with various features and functionalities.

## Quick Start

Here's how you can get started with Endstone:

1. **Create a virtual environment**

   In your terminal, navigate to the directory where you want to create your virtual environment and run:

   ```bash
   python -m venv venv
   ```

   This creates a new virtual environment in a folder named `venv`.

2. **Activate the virtual environment**

   On Linux:

   ```bash
   source venv/bin/activate
   ```

   On Windows:

   ```bash
   .\venv\Scripts\activate
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
      pip install -e .
      ```

4. **Start the server**

   Run the following command to start the server:

   ```bash
   endstone
   ```

Congratulations! You're now ready to use Endstone to create plugins for Bedrock Dedicated Servers.