

# Class endstone::PluginDescription



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginDescription**](classendstone_1_1PluginDescription.md)



_Represents the basic information about a plugin that the plugin loader needs to know._ 

* `#include <endstone/plugin/plugin_description.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginDescription**](#function-plugindescription) (std::string name, std::string version, std::string description="", [**PluginLoadOrder**](namespaceendstone.md#enum-pluginloadorder) load=PluginLoadOrder::PostWorld, std::vector&lt; std::string &gt; authors={}, std::vector&lt; std::string &gt; contributors={}, std::string website="", std::string prefix="", std::vector&lt; std::string &gt; provides={}, std::vector&lt; std::string &gt; depend={}, std::vector&lt; std::string &gt; soft\_depend={}, std::vector&lt; std::string &gt; load\_before={}, [**PermissionDefault**](namespaceendstone.md#enum-permissiondefault) default\_permission=PermissionDefault::Operator, std::vector&lt; [**Command**](classendstone_1_1Command.md) &gt; commands={}, std::vector&lt; [**Permission**](classendstone_1_1Permission.md) &gt; permissions={}) <br> |
|  std::string | [**getAPIVersion**](#function-getapiversion) () const<br>_Gives the API version which this plugin is designed to support._  |
|  std::vector&lt; std::string &gt; | [**getAuthors**](#function-getauthors) () const<br>_Gives the list of authors for the plugin._  |
|  std::vector&lt; [**Command**](classendstone_1_1Command.md) &gt; | [**getCommands**](#function-getcommands) () const<br>_Gives the list of commands the plugin will register at runtime._  |
|  std::vector&lt; std::string &gt; | [**getContributors**](#function-getcontributors) () const<br>_Gives the list of contributors for the plugin._  |
|  [**PermissionDefault**](namespaceendstone.md#enum-permissiondefault) | [**getDefaultPermission**](#function-getdefaultpermission) () const<br>_Gives the default value of permissions registered for the plugin._  |
|  std::vector&lt; std::string &gt; | [**getDepend**](#function-getdepend) () const<br>_Gives a list of other plugins that the plugin requires._  |
|  std::string | [**getDescription**](#function-getdescription) () const<br>_Gives a human-friendly description of the functionality the plugin provides._  |
|  std::string | [**getFullName**](#function-getfullname) () const<br>_Returns the name of a plugin, including the version._  |
|  [**PluginLoadOrder**](namespaceendstone.md#enum-pluginloadorder) | [**getLoad**](#function-getload) () const<br>_Gives the phase of server startup that the plugin should be loaded._  |
|  std::vector&lt; std::string &gt; | [**getLoadBefore**](#function-getloadbefore) () const<br>_Gets the list of plugins that should consider this plugin a soft-dependency._  |
|  std::string | [**getName**](#function-getname) () const<br>_Gives the name of the plugin._  |
|  std::vector&lt; [**Permission**](classendstone_1_1Permission.md) &gt; | [**getPermissions**](#function-getpermissions) () const<br>_Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling._  |
|  std::string | [**getPrefix**](#function-getprefix) () const<br>_Gives the token to prefix plugin-specific logging messages with._  |
|  std::vector&lt; std::string &gt; | [**getProvides**](#function-getprovides) () const<br>_Gives the list of other plugin APIs which this plugin provides._  |
|  std::vector&lt; std::string &gt; | [**getSoftDepend**](#function-getsoftdepend) () const<br>_Gives a list of other plugins that the plugin requires for full functionality._  |
|  std::string | [**getVersion**](#function-getversion) () const<br>_Gives the version of the plugin._  |
|  std::string | [**getWebsite**](#function-getwebsite) () const<br>_Gives the plugin's or plugin's author's website._  |




























## Public Functions Documentation




### function PluginDescription 

```C++
inline endstone::PluginDescription::PluginDescription (
    std::string name,
    std::string version,
    std::string description="",
    PluginLoadOrder load=PluginLoadOrder::PostWorld,
    std::vector< std::string > authors={},
    std::vector< std::string > contributors={},
    std::string website="",
    std::string prefix="",
    std::vector< std::string > provides={},
    std::vector< std::string > depend={},
    std::vector< std::string > soft_depend={},
    std::vector< std::string > load_before={},
    PermissionDefault default_permission=PermissionDefault::Operator,
    std::vector< Command > commands={},
    std::vector< Permission > permissions={}
) 
```




<hr>



### function getAPIVersion 

_Gives the API version which this plugin is designed to support._ 
```C++
inline std::string endstone::PluginDescription::getAPIVersion () const
```





**Returns:**

the API version supported by the plugin 





        

<hr>



### function getAuthors 

_Gives the list of authors for the plugin._ 
```C++
inline std::vector< std::string > endstone::PluginDescription::getAuthors () const
```





**Returns:**

an immutable list of the plugin's authors 





        

<hr>



### function getCommands 

_Gives the list of commands the plugin will register at runtime._ 
```C++
inline std::vector< Command > endstone::PluginDescription::getCommands () const
```





**Returns:**

the commands this plugin will register 





        

<hr>



### function getContributors 

_Gives the list of contributors for the plugin._ 
```C++
inline std::vector< std::string > endstone::PluginDescription::getContributors () const
```





**Returns:**

an immutable list of the plugin's contributions 





        

<hr>



### function getDefaultPermission 

_Gives the default value of permissions registered for the plugin._ 
```C++
inline PermissionDefault endstone::PluginDescription::getDefaultPermission () const
```





**Returns:**

the default value for the plugin's permissions 





        

<hr>



### function getDepend 

_Gives a list of other plugins that the plugin requires._ 
```C++
inline std::vector< std::string > endstone::PluginDescription::getDepend () const
```





**Returns:**

immutable list of the plugin's dependencies 





        

<hr>



### function getDescription 

_Gives a human-friendly description of the functionality the plugin provides._ 
```C++
inline std::string endstone::PluginDescription::getDescription () const
```





**Returns:**

description of this plugin, or empty if not specified 





        

<hr>



### function getFullName 

_Returns the name of a plugin, including the version._ 
```C++
inline std::string endstone::PluginDescription::getFullName () const
```





**Returns:**

a descriptive name of the plugin and respective version 





        

<hr>



### function getLoad 

_Gives the phase of server startup that the plugin should be loaded._ 
```C++
inline PluginLoadOrder endstone::PluginDescription::getLoad () const
```





**Returns:**

the phase when the plugin should be loaded 





        

<hr>



### function getLoadBefore 

_Gets the list of plugins that should consider this plugin a soft-dependency._ 
```C++
inline std::vector< std::string > endstone::PluginDescription::getLoadBefore () const
```





**Returns:**

immutable list of plugins that should consider this plugin a soft-dependency 





        

<hr>



### function getName 

_Gives the name of the plugin._ 
```C++
inline std::string endstone::PluginDescription::getName () const
```



This name is a unique identifier for plugins.




**Returns:**

the name of the plugin 





        

<hr>



### function getPermissions 

_Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling._ 
```C++
inline std::vector< Permission > endstone::PluginDescription::getPermissions () const
```





**Returns:**

the permissions this plugin will register 





        

<hr>



### function getPrefix 

_Gives the token to prefix plugin-specific logging messages with._ 
```C++
inline std::string endstone::PluginDescription::getPrefix () const
```





**Returns:**

the prefixed logging token, or empty if not specified 





        

<hr>



### function getProvides 

_Gives the list of other plugin APIs which this plugin provides._ 
```C++
inline std::vector< std::string > endstone::PluginDescription::getProvides () const
```



These are usable for other plugins to depend on.




**Returns:**

immutable list of the plugin APIs which this plugin provides 





        

<hr>



### function getSoftDepend 

_Gives a list of other plugins that the plugin requires for full functionality._ 
```C++
inline std::vector< std::string > endstone::PluginDescription::getSoftDepend () const
```





**Returns:**

immutable list of the plugin's preferred dependencies 





        

<hr>



### function getVersion 

_Gives the version of the plugin._ 
```C++
inline std::string endstone::PluginDescription::getVersion () const
```





**Returns:**

the version of the plugin 





        

<hr>



### function getWebsite 

_Gives the plugin's or plugin's author's website._ 
```C++
inline std::string endstone::PluginDescription::getWebsite () const
```





**Returns:**

the website of this plugin, or empty if not specified 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin_description.h`

