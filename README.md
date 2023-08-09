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

## Creating a Sample Plugin

To get a taste of what you can do with Endstone, let's create a simple plugin. This will give you an idea of the basic
structure of a plugin and how it interacts with the server.

Here's a step-by-step guide to creating a sample plugin:

1. **Create a new folder for the plugin**

   Under the folder named `plugins` in the server directory, create a new folder named `sample_plugin`.

2. **Create a Python file for the plugin**

   Inside the `sample_plugin` folder, create a Python file named `sample_plugin.py` and add the following code:

   ```python
   from endstone.plugin import Plugin

   class SamplePlugin(Plugin):

       def on_load(self):
           self.logger.info("on_load is called!")

       def on_enable(self):
           self.logger.info("on_enable is called!")

       def on_disable(self):
           self.logger.info("on_disable is called!")
   ```

3. **Create a configuration file for the plugin**

   Under the same folder, create a file named `plugin.toml` and include the following:

   ```toml
   name = "Sample Plugin"
   version = "1.0.0"
   main = "sample_plugin.sample_plugin.SamplePlugin"
   ```

   Here, the main field specifies the location of the main plugin class. It's constructed by combining the folder
   name `sample_plugin`, the file name `sample_plugin.py` (without the extension), and the class name `SamplePlugin`.

4. **Run the server again**

   Start the server again using the command:

   ```bash
   endstone
   ```

   You should now see the sample plugin being loaded, and the corresponding log messages will appear in the server's
   console.

Congratulations! You've just created a simple plugin for Endstone. You are now ready to extend this sample to include
more complex features.

## Next steps

Explore the Endstone documentation (WIP) for more in-depth guides and resources to help you
create powerful plugins for Bedrock Dedicated Servers.
