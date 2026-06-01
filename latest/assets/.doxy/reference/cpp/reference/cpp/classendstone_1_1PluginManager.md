

# Class endstone::PluginManager



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginManager**](classendstone_1_1PluginManager.md)



_Represents a plugin manager that handles all plugins from the_ [_**Server**_](classendstone_1_1Server.md) _._

* `#include <endstone/plugin/plugin_manager.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginManager**](#function-pluginmanager-12) () = default<br> |
|   | [**PluginManager**](#function-pluginmanager-22) ([**PluginManager**](classendstone_1_1PluginManager.md) [**const**](classendstone_1_1Identifier.md) &) = delete<br> |
| virtual [**Permission**](classendstone_1_1Permission.md) & | [**addPermission**](#function-addpermission) (std::unique\_ptr&lt; [**Permission**](classendstone_1_1Permission.md) &gt; perm) = 0<br>_Adds a_ [_**Permission**_](classendstone_1_1Permission.md) _to this plugin manager._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**callEvent**](#function-callevent) ([**Event**](classendstone_1_1Event.md) & event) = 0<br>_Calls an event which will be passed to plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**clearPlugins**](#function-clearplugins) () = 0<br>_Disables and removes all plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**disablePlugin**](#function-disableplugin) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Disables the specified plugin._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**disablePlugins**](#function-disableplugins) () = 0<br>_Disables all the loaded plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**enablePlugin**](#function-enableplugin) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) const = 0<br>_Enables the specified plugin._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**enablePlugins**](#function-enableplugins) () const = 0<br>_Enable all the loaded plugins._  |
| virtual std::unordered\_set&lt; [**Permissible**](classendstone_1_1Permissible.md) \* &gt; | [**getDefaultPermSubscriptions**](#function-getdefaultpermsubscriptions) ([**PermissionLevel**](namespaceendstone.md#enum-permissionlevel) level) const = 0<br>_Gets a set containing all subscribed Permissibles to the given default list, by permission level._  |
| virtual std::vector&lt; [**Permission**](classendstone_1_1Permission.md) \* &gt; | [**getDefaultPermissions**](#function-getdefaultpermissions) ([**PermissionLevel**](namespaceendstone.md#enum-permissionlevel) level) const = 0<br>_Gets the default permissions for the given permission level._  |
| virtual [**Permission**](classendstone_1_1Permission.md) \* | [**getPermission**](#function-getpermission) (std::string name) const = 0<br>_Gets a_ [_**Permission**_](classendstone_1_1Permission.md) _from its fully qualified name._ |
| virtual std::unordered\_set&lt; [**Permissible**](classendstone_1_1Permissible.md) \* &gt; | [**getPermissionSubscriptions**](#function-getpermissionsubscriptions) (std::string permission) const = 0<br>_Gets a set containing all subscribed Permissibles to the given permission, by name._  |
| virtual std::unordered\_set&lt; [**Permission**](classendstone_1_1Permission.md) \* &gt; | [**getPermissions**](#function-getpermissions) () const = 0<br>_Gets a set of all registered permissions._  |
| virtual [**Plugin**](classendstone_1_1Plugin.md) \* | [**getPlugin**](#function-getplugin) ([**const**](classendstone_1_1Identifier.md) std::string & name) const = 0<br>_Checks if the given plugin is loaded and returns it when applicable._  |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**getPlugins**](#function-getplugins) () const = 0<br>_Gets a list of all currently loaded plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isPluginEnabled**](#function-ispluginenabled-12) ([**const**](classendstone_1_1Identifier.md) std::string & name) const = 0<br>_Checks if the given plugin is enabled or not._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isPluginEnabled**](#function-ispluginenabled-22) ([**Plugin**](classendstone_1_1Plugin.md) \* plugin) const = 0<br>_Checks if the given plugin is enabled or not._  |
| virtual [**Plugin**](classendstone_1_1Plugin.md) \* | [**loadPlugin**](#function-loadplugin) (std::string file) = 0<br>_Loads the plugin in the specified file._  |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**loadPlugins**](#function-loadplugins-12) (std::string directory) = 0<br>_Loads the plugin contained within the specified directory._  |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**loadPlugins**](#function-loadplugins-22) (std::vector&lt; std::string &gt; files) = 0<br>_Loads the plugins in the list of the files._  |
|  [**PluginManager**](classendstone_1_1PluginManager.md) & | [**operator=**](#function-operator) ([**PluginManager**](classendstone_1_1PluginManager.md) [**const**](classendstone_1_1Identifier.md) &) = delete<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**recalculatePermissionDefaults**](#function-recalculatepermissiondefaults) ([**Permission**](classendstone_1_1Permission.md) & perm) = 0<br>_Recalculates the defaults for the given_ `Permission` _._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**registerEvent**](#function-registerevent) (std::string event, std::function&lt; [**void**](classendstone_1_1Identifier.md)([**Event**](classendstone_1_1Event.md) &)&gt; executor, [**EventPriority**](namespaceendstone.md#enum-eventpriority) priority, [**Plugin**](classendstone_1_1Plugin.md) & plugin, [**bool**](classendstone_1_1Identifier.md) ignore\_cancelled) = 0<br>_Registers the given event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**registerLoader**](#function-registerloader) (std::unique\_ptr&lt; [**PluginLoader**](classendstone_1_1PluginLoader.md) &gt; loader) = 0<br>_Registers the specified plugin loader._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removePermission**](#function-removepermission-12) ([**Permission**](classendstone_1_1Permission.md) & perm) = 0<br>_Removes a_ [_**Permission**_](classendstone_1_1Permission.md) _registration from this plugin manager._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removePermission**](#function-removepermission-22) (std::string name) = 0<br>_Removes a_ [_**Permission**_](classendstone_1_1Permission.md) _registration from this plugin manager._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**subscribeToDefaultPerms**](#function-subscribetodefaultperms) ([**PermissionLevel**](namespaceendstone.md#enum-permissionlevel) level, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br>_Subscribes to the given Default permissions by permission level._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**subscribeToPermission**](#function-subscribetopermission) (std::string permission, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br>_Subscribes the given_ [_**Permissible**_](classendstone_1_1Permissible.md) _for information about the requested_[_**Permission**_](classendstone_1_1Permission.md) _, by name._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**unsubscribeFromDefaultPerms**](#function-unsubscribefromdefaultperms) ([**PermissionLevel**](namespaceendstone.md#enum-permissionlevel) level, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br>_Unsubscribes from the given Default permissions by permission level._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**unsubscribeFromPermission**](#function-unsubscribefrompermission) (std::string permission, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br>_Unsubscribes the given_ [_**Permissible**_](classendstone_1_1Permissible.md) _for information about the requested_[_**Permission**_](classendstone_1_1Permission.md) _, by name._ |
| virtual  | [**~PluginManager**](#function-pluginmanager) () = default<br> |




























## Public Functions Documentation




### function PluginManager [1/2]

```C++
endstone::PluginManager::PluginManager () = default
```




<hr>



### function PluginManager [2/2]

```C++
endstone::PluginManager::PluginManager (
    PluginManager  const &
) = delete
```




<hr>



### function addPermission 

_Adds a_ [_**Permission**_](classendstone_1_1Permission.md) _to this plugin manager._
```C++
virtual Permission & endstone::PluginManager::addPermission (
    std::unique_ptr< Permission > perm
) = 0
```





**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to add 



**Returns:**

A reference to the newly added permission. 




**Exception:**


* `std::runtime_error` if a permission with the same name is already defined. 




        

<hr>



### function callEvent 

_Calls an event which will be passed to plugins._ 
```C++
virtual void endstone::PluginManager::callEvent (
    Event & event
) = 0
```





**Parameters:**


* `event` [**Event**](classendstone_1_1Event.md) to be called 




        

<hr>



### function clearPlugins 

_Disables and removes all plugins._ 
```C++
virtual void endstone::PluginManager::clearPlugins () = 0
```




<hr>



### function disablePlugin 

_Disables the specified plugin._ 
```C++
virtual void endstone::PluginManager::disablePlugin (
    Plugin & plugin
) = 0
```



Attempting to disable a plugin that is not enabled will have no effect.




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to disable 




        

<hr>



### function disablePlugins 

_Disables all the loaded plugins._ 
```C++
virtual void endstone::PluginManager::disablePlugins () = 0
```




<hr>



### function enablePlugin 

_Enables the specified plugin._ 
```C++
virtual void endstone::PluginManager::enablePlugin (
    Plugin & plugin
) const = 0
```



Attempting to enable a plugin that is already enabled will have no effect.




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to enable 




        

<hr>



### function enablePlugins 

_Enable all the loaded plugins._ 
```C++
virtual void endstone::PluginManager::enablePlugins () const = 0
```




<hr>



### function getDefaultPermSubscriptions 

_Gets a set containing all subscribed Permissibles to the given default list, by permission level._ 
```C++
virtual std::unordered_set< Permissible * > endstone::PluginManager::getDefaultPermSubscriptions (
    PermissionLevel level
) const = 0
```





**Parameters:**


* `level` Default list to query for 



**Returns:**

Set containing all subscribed permissions 





        

<hr>



### function getDefaultPermissions 

_Gets the default permissions for the given permission level._ 
```C++
virtual std::vector< Permission * > endstone::PluginManager::getDefaultPermissions (
    PermissionLevel level
) const = 0
```





**Parameters:**


* `level` Which set of default permissions to get 



**Returns:**

The default permissions 





        

<hr>



### function getPermission 

_Gets a_ [_**Permission**_](classendstone_1_1Permission.md) _from its fully qualified name._
```C++
virtual Permission * endstone::PluginManager::getPermission (
    std::string name
) const = 0
```





**Parameters:**


* `name` Name of the permission 



**Returns:**

[**Permission**](classendstone_1_1Permission.md), or nullptr if none 





        

<hr>



### function getPermissionSubscriptions 

_Gets a set containing all subscribed Permissibles to the given permission, by name._ 
```C++
virtual std::unordered_set< Permissible * > endstone::PluginManager::getPermissionSubscriptions (
    std::string permission
) const = 0
```





**Parameters:**


* `permission` [**Permission**](classendstone_1_1Permission.md) to query for 



**Returns:**

Set containing all subscribed permissions 





        

<hr>



### function getPermissions 

_Gets a set of all registered permissions._ 
```C++
virtual std::unordered_set< Permission * > endstone::PluginManager::getPermissions () const = 0
```



This set is a copy and will not be modified live.




**Returns:**

Set containing all current registered permissions 





        

<hr>



### function getPlugin 

_Checks if the given plugin is loaded and returns it when applicable._ 
```C++
virtual Plugin * endstone::PluginManager::getPlugin (
    const std::string & name
) const = 0
```



Please note that the name of the plugin is case-sensitive.




**Parameters:**


* `name` Name of the plugin to check 



**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) if it exists, otherwise nullptr 





        

<hr>



### function getPlugins 

_Gets a list of all currently loaded plugins._ 
```C++
virtual std::vector< Plugin * > endstone::PluginManager::getPlugins () const = 0
```





**Returns:**

List of Plugins 





        

<hr>



### function isPluginEnabled [1/2]

_Checks if the given plugin is enabled or not._ 
```C++
virtual bool endstone::PluginManager::isPluginEnabled (
    const std::string & name
) const = 0
```



Please note that the name of the plugin is case-sensitive.




**Parameters:**


* `name` Name of the plugin to check 



**Returns:**

true if the plugin is enabled, otherwise false 





        

<hr>



### function isPluginEnabled [2/2]

_Checks if the given plugin is enabled or not._ 
```C++
virtual bool endstone::PluginManager::isPluginEnabled (
    Plugin * plugin
) const = 0
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to check 



**Returns:**

true if the plugin is enabled, otherwise false 





        

<hr>



### function loadPlugin 

_Loads the plugin in the specified file._ 
```C++
virtual Plugin * endstone::PluginManager::loadPlugin (
    std::string file
) = 0
```



File must be valid according to the current enabled [**Plugin**](classendstone_1_1Plugin.md) interfaces




**Parameters:**


* `file` File containing the plugin to load 



**Returns:**

The [**Plugin**](classendstone_1_1Plugin.md) loaded, or nullptr if it was invalid 





        

<hr>



### function loadPlugins [1/2]

_Loads the plugin contained within the specified directory._ 
```C++
virtual std::vector< Plugin * > endstone::PluginManager::loadPlugins (
    std::string directory
) = 0
```





**Parameters:**


* `directory` Directory to check for plugins 



**Returns:**

A list of all plugins loaded 





        

<hr>



### function loadPlugins [2/2]

_Loads the plugins in the list of the files._ 
```C++
virtual std::vector< Plugin * > endstone::PluginManager::loadPlugins (
    std::vector< std::string > files
) = 0
```





**Parameters:**


* `files` List of files containing plugins to load 



**Returns:**

A list of all plugins loaded 





        

<hr>



### function operator= 

```C++
PluginManager & endstone::PluginManager::operator= (
    PluginManager  const &
) = delete
```




<hr>



### function recalculatePermissionDefaults 

_Recalculates the defaults for the given_ `Permission` _._
```C++
virtual void endstone::PluginManager::recalculatePermissionDefaults (
    Permission & perm
) = 0
```



This will have no effect if the specified permission is not registered here.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to recalculate 




        

<hr>



### function registerEvent 

_Registers the given event._ 
```C++
virtual void endstone::PluginManager::registerEvent (
    std::string event,
    std::function< void ( Event &)> executor,
    EventPriority priority,
    Plugin & plugin,
    bool ignore_cancelled
) = 0
```





**Parameters:**


* `event` [**Event**](classendstone_1_1Event.md) name to register 
* `executor` EventExecutor to register 
* `priority` Priority of this event 
* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to register 
* `ignore_cancelled` Do not call executor if event was already cancelled 




        

<hr>



### function registerLoader 

_Registers the specified plugin loader._ 
```C++
virtual void endstone::PluginManager::registerLoader (
    std::unique_ptr< PluginLoader > loader
) = 0
```





**Parameters:**


* `loader` [**PluginLoader**](classendstone_1_1PluginLoader.md) to register 




        

<hr>



### function removePermission [1/2]

_Removes a_ [_**Permission**_](classendstone_1_1Permission.md) _registration from this plugin manager._
```C++
virtual void endstone::PluginManager::removePermission (
    Permission & perm
) = 0
```



If the specified permission does not exist in this plugin manager, nothing will happen. Removing a permission registration will **not** remove the permission from any Permissibles that have it.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>



### function removePermission [2/2]

_Removes a_ [_**Permission**_](classendstone_1_1Permission.md) _registration from this plugin manager._
```C++
virtual void endstone::PluginManager::removePermission (
    std::string name
) = 0
```



If the specified permission does not exist in this plugin manager, nothing will happen. Removing a permission registration will **not** remove the permission from any Permissibles that have it.




**Parameters:**


* `name` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>



### function subscribeToDefaultPerms 

_Subscribes to the given Default permissions by permission level._ 
```C++
virtual void endstone::PluginManager::subscribeToDefaultPerms (
    PermissionLevel level,
    Permissible & permissible
) = 0
```



If the specified defaults change in any form, the [**Permissible**](classendstone_1_1Permissible.md) will be asked to recalculate.




**Parameters:**


* `level` Default list to subscribe to 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function subscribeToPermission 

_Subscribes the given_ [_**Permissible**_](classendstone_1_1Permissible.md) _for information about the requested_[_**Permission**_](classendstone_1_1Permission.md) _, by name._
```C++
virtual void endstone::PluginManager::subscribeToPermission (
    std::string permission,
    Permissible & permissible
) = 0
```



If the specified [**Permission**](classendstone_1_1Permission.md) changes in any form, the [**Permissible**](classendstone_1_1Permissible.md) will be asked to recalculate.




**Parameters:**


* `permission` [**Permission**](classendstone_1_1Permission.md) to subscribe to 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function unsubscribeFromDefaultPerms 

_Unsubscribes from the given Default permissions by permission level._ 
```C++
virtual void endstone::PluginManager::unsubscribeFromDefaultPerms (
    PermissionLevel level,
    Permissible & permissible
) = 0
```





**Parameters:**


* `level` Default list to unsubscribe from 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function unsubscribeFromPermission 

_Unsubscribes the given_ [_**Permissible**_](classendstone_1_1Permissible.md) _for information about the requested_[_**Permission**_](classendstone_1_1Permission.md) _, by name._
```C++
virtual void endstone::PluginManager::unsubscribeFromPermission (
    std::string permission,
    Permissible & permissible
) = 0
```





**Parameters:**


* `permission` [**Permission**](classendstone_1_1Permission.md) to unsubscribe from 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function ~PluginManager 

```C++
virtual endstone::PluginManager::~PluginManager () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin_manager.h`

