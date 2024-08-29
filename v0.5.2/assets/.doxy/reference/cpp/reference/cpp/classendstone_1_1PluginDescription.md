

# Class endstone::PluginDescription



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginDescription**](classendstone_1_1PluginDescription.md)



_Represents the basic information about a plugin that the plugin loader needs to know._ 

* `#include <endstone/plugin/plugin_description.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginDescription**](#function-plugindescription) (std::string name, std::string version, std::string description="", PluginLoadOrder load=PluginLoadOrder::PostWorld, std::vector&lt; std::string &gt; authors={}, std::vector&lt; std::string &gt; contributors={}, std::string website="", std::string prefix="", std::vector&lt; std::string &gt; provides={}, std::vector&lt; std::string &gt; depend={}, std::vector&lt; std::string &gt; soft\_depend={}, std::vector&lt; std::string &gt; load\_before={}, PermissionDefault default\_permission=PermissionDefault::Operator, std::vector&lt; [**Command**](classendstone_1_1Command.md) &gt; commands={}, std::vector&lt; [**Permission**](classendstone_1_1Permission.md) &gt; permissions={}) <br> |
|  std::string | [**getAPIVersion**](#function-getapiversion) () const<br> |
|  std::vector&lt; std::string &gt; | [**getAuthors**](#function-getauthors) () const<br> |
|  std::vector&lt; [**Command**](classendstone_1_1Command.md) &gt; | [**getCommands**](#function-getcommands) () const<br> |
|  std::vector&lt; std::string &gt; | [**getContributors**](#function-getcontributors) () const<br> |
|  PermissionDefault | [**getDefaultPermission**](#function-getdefaultpermission) () const<br> |
|  std::vector&lt; std::string &gt; | [**getDepend**](#function-getdepend) () const<br> |
|  std::string | [**getDescription**](#function-getdescription) () const<br> |
|  std::string | [**getFullName**](#function-getfullname) () const<br> |
|  PluginLoadOrder | [**getLoad**](#function-getload) () const<br> |
|  std::vector&lt; std::string &gt; | [**getLoadBefore**](#function-getloadbefore) () const<br> |
|  std::string | [**getName**](#function-getname) () const<br> |
|  std::vector&lt; [**Permission**](classendstone_1_1Permission.md) &gt; | [**getPermissions**](#function-getpermissions) () const<br> |
|  std::string | [**getPrefix**](#function-getprefix) () const<br> |
|  std::vector&lt; std::string &gt; | [**getProvides**](#function-getprovides) () const<br> |
|  std::vector&lt; std::string &gt; | [**getSoftDepend**](#function-getsoftdepend) () const<br> |
|  std::string | [**getVersion**](#function-getversion) () const<br> |
|  std::string | [**getWebsite**](#function-getwebsite) () const<br> |




























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


```C++
inline std::string endstone::PluginDescription::getAPIVersion () const
```



Gives the API version which this plugin is designed to support.




**Returns:**

the API version supported by the plugin 





        

<hr>



### function getAuthors 


```C++
inline std::vector< std::string > endstone::PluginDescription::getAuthors () const
```



Gives the list of authors for the plugin.




**Returns:**

an immutable list of the plugin's authors 





        

<hr>



### function getCommands 


```C++
inline std::vector< Command > endstone::PluginDescription::getCommands () const
```



Gives the list of commands the plugin will register at runtime.




**Returns:**

the commands this plugin will register 





        

<hr>



### function getContributors 


```C++
inline std::vector< std::string > endstone::PluginDescription::getContributors () const
```



Gives the list of contributors for the plugin.




**Returns:**

an immutable list of the plugin's contributions 





        

<hr>



### function getDefaultPermission 


```C++
inline PermissionDefault endstone::PluginDescription::getDefaultPermission () const
```



Gives the default value of permissions registered for the plugin.




**Returns:**

the default value for the plugin's permissions 





        

<hr>



### function getDepend 


```C++
inline std::vector< std::string > endstone::PluginDescription::getDepend () const
```



Gives a list of other plugins that the plugin requires.




**Returns:**

immutable list of the plugin's dependencies 





        

<hr>



### function getDescription 


```C++
inline std::string endstone::PluginDescription::getDescription () const
```



Gives a human-friendly description of the functionality the plugin provides. 

**Returns:**

description of this plugin, or empty if not specified 





        

<hr>



### function getFullName 


```C++
inline std::string endstone::PluginDescription::getFullName () const
```



Returns the name of a plugin, including the version.




**Returns:**

a descriptive name of the plugin and respective version 





        

<hr>



### function getLoad 


```C++
inline PluginLoadOrder endstone::PluginDescription::getLoad () const
```



Gives the phase of server startup that the plugin should be loaded.




**Returns:**

the phase when the plugin should be loaded 





        

<hr>



### function getLoadBefore 


```C++
inline std::vector< std::string > endstone::PluginDescription::getLoadBefore () const
```



Gets the list of plugins that should consider this plugin a soft-dependency.




**Returns:**

immutable list of plugins that should consider this plugin a soft-dependency 





        

<hr>



### function getName 


```C++
inline std::string endstone::PluginDescription::getName () const
```



Gives the name of the plugin. This name is a unique identifier for plugins.




**Returns:**

the name of the plugin 





        

<hr>



### function getPermissions 


```C++
inline std::vector< Permission > endstone::PluginDescription::getPermissions () const
```



Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.




**Returns:**

the permissions this plugin will register 





        

<hr>



### function getPrefix 


```C++
inline std::string endstone::PluginDescription::getPrefix () const
```



Gives the token to prefix plugin-specific logging messages with.




**Returns:**

the prefixed logging token, or empty if not specified 





        

<hr>



### function getProvides 


```C++
inline std::vector< std::string > endstone::PluginDescription::getProvides () const
```



Gives the list of other plugin APIs which this plugin provides. These are usable for other plugins to depend on.




**Returns:**

immutable list of the plugin APIs which this plugin provides 





        

<hr>



### function getSoftDepend 


```C++
inline std::vector< std::string > endstone::PluginDescription::getSoftDepend () const
```



Gives a list of other plugins that the plugin requires for full functionality.




**Returns:**

immutable list of the plugin's preferred dependencies 





        

<hr>



### function getVersion 


```C++
inline std::string endstone::PluginDescription::getVersion () const
```



Gives the version of the plugin.




**Returns:**

the version of the plugin 





        

<hr>



### function getWebsite 


```C++
inline std::string endstone::PluginDescription::getWebsite () const
```



Gives the plugin's or plugin's author's website.




**Returns:**

the website of this plugin, or empty if not specified 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin_description.h`

