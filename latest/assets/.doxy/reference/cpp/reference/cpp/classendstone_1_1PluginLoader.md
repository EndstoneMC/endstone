

# Class endstone::PluginLoader



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginLoader**](classendstone_1_1PluginLoader.md)



_Represents a plugin loader, which handles direct access to specific types of plugins._ 

* `#include <endstone/plugin/plugin_loader.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginLoader**](#function-pluginloader-12) ([**Server**](classendstone_1_1Server.md) & server) <br> |
|   | [**PluginLoader**](#function-pluginloader-22) (const [**PluginLoader**](classendstone_1_1PluginLoader.md) &) = delete<br> |
| virtual void | [**disablePlugin**](#function-disableplugin) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) const<br>_Disables the specified plugin Attempting to disable a plugin that is not enabled will have no effect._  |
| virtual void | [**enablePlugin**](#function-enableplugin) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) const<br>_Enables the specified plugin Attempting to enable a plugin that is already enabled will have no effect._  |
| virtual std::vector&lt; std::string &gt; | [**getPluginFileFilters**](#function-getpluginfilefilters) () const = 0<br>_Returns a list of all filename filters expected by this_ [_**PluginLoader**_](classendstone_1_1PluginLoader.md) _._ |
|  [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () const<br>_Retrieves the_ [_**Server**_](classendstone_1_1Server.md) _object associated with the_[_**PluginLoader**_](classendstone_1_1PluginLoader.md) _._ |
| virtual [**Plugin**](classendstone_1_1Plugin.md) \* | [**loadPlugin**](#function-loadplugin) (std::string file) = 0<br> |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**loadPlugins**](#function-loadplugins) (std::string directory) <br>_Loads the plugin contained within the specified directory._  |
|  [**PluginLoader**](classendstone_1_1PluginLoader.md) & | [**operator=**](#function-operator) (const [**PluginLoader**](classendstone_1_1PluginLoader.md) &) = delete<br> |
| virtual  | [**~PluginLoader**](#function-pluginloader) () = default<br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  [**Server**](classendstone_1_1Server.md) & | [**server\_**](#variable-server_)  <br> |




















## Public Functions Documentation




### function PluginLoader [1/2]

```C++
inline explicit endstone::PluginLoader::PluginLoader (
    Server & server
) 
```




<hr>



### function PluginLoader [2/2]

```C++
endstone::PluginLoader::PluginLoader (
    const PluginLoader &
) = delete
```




<hr>



### function disablePlugin 

_Disables the specified plugin Attempting to disable a plugin that is not enabled will have no effect._ 
```C++
inline virtual void endstone::PluginLoader::disablePlugin (
    Plugin & plugin
) const
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to disable 




        

<hr>



### function enablePlugin 

_Enables the specified plugin Attempting to enable a plugin that is already enabled will have no effect._ 
```C++
inline virtual void endstone::PluginLoader::enablePlugin (
    Plugin & plugin
) const
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to enable 




        

<hr>



### function getPluginFileFilters 

_Returns a list of all filename filters expected by this_ [_**PluginLoader**_](classendstone_1_1PluginLoader.md) _._
```C++
virtual std::vector< std::string > endstone::PluginLoader::getPluginFileFilters () const = 0
```





**Returns:**

The filters 





        

<hr>



### function getServer 

_Retrieves the_ [_**Server**_](classendstone_1_1Server.md) _object associated with the_[_**PluginLoader**_](classendstone_1_1PluginLoader.md) _._
```C++
inline Server & endstone::PluginLoader::getServer () const
```



This function returns a reference to the [**Server**](classendstone_1_1Server.md) object that the [**PluginLoader**](classendstone_1_1PluginLoader.md) is associated with.




**Returns:**

The [**Server**](classendstone_1_1Server.md) reference. 





        

<hr>



### function loadPlugin 

```C++
virtual Plugin * endstone::PluginLoader::loadPlugin (
    std::string file
) = 0
```



Loads the plugin contained in the specified file




**Parameters:**


* `file` File to attempt to load 



**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) that was contained in the specified file, or nullptr if unsuccessful 





        

<hr>



### function loadPlugins 

_Loads the plugin contained within the specified directory._ 
```C++
inline virtual std::vector< Plugin * > endstone::PluginLoader::loadPlugins (
    std::string directory
) 
```





**Parameters:**


* `directory` Directory to check for plugins 



**Returns:**

A list of all plugins loaded 





        

<hr>



### function operator= 

```C++
PluginLoader & endstone::PluginLoader::operator= (
    const PluginLoader &
) = delete
```




<hr>



### function ~PluginLoader 

```C++
virtual endstone::PluginLoader::~PluginLoader () = default
```




<hr>
## Protected Attributes Documentation




### variable server\_ 

```C++
Server& endstone::PluginLoader::server_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin_loader.h`

