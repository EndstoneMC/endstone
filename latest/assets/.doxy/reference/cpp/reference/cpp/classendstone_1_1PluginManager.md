

# Class endstone::PluginManager



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginManager**](classendstone_1_1PluginManager.md)



_Represents a plugin manager that handles all plugins from the_ [_**Server**_](classendstone_1_1Server.md) _._

* `#include <endstone/plugin/plugin_manager.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginManager**](#function-pluginmanager-12) () = default<br> |
|   | [**PluginManager**](#function-pluginmanager-22) ([**PluginManager**](classendstone_1_1PluginManager.md) const &) = delete<br> |
| virtual [**Permission**](classendstone_1_1Permission.md) \* | [**addPermission**](#function-addpermission) (std::unique\_ptr&lt; [**Permission**](classendstone_1_1Permission.md) &gt; perm) = 0<br> |
| virtual void | [**callEvent**](#function-callevent) ([**Event**](classendstone_1_1Event.md) & event) = 0<br> |
| virtual void | [**clearPlugins**](#function-clearplugins) () = 0<br> |
| virtual void | [**disablePlugin**](#function-disableplugin) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual void | [**disablePlugins**](#function-disableplugins) () = 0<br> |
| virtual void | [**enablePlugin**](#function-enableplugin) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) const = 0<br> |
| virtual void | [**enablePlugins**](#function-enableplugins) () const = 0<br> |
| virtual std::unordered\_set&lt; [**Permissible**](classendstone_1_1Permissible.md) \* &gt; | [**getDefaultPermSubscriptions**](#function-getdefaultpermsubscriptions) (bool op) const = 0<br> |
| virtual std::unordered\_set&lt; [**Permission**](classendstone_1_1Permission.md) \* &gt; | [**getDefaultPermissions**](#function-getdefaultpermissions) (bool op) const = 0<br> |
| virtual [**Permission**](classendstone_1_1Permission.md) \* | [**getPermission**](#function-getpermission) (std::string name) const = 0<br> |
| virtual std::unordered\_set&lt; [**Permissible**](classendstone_1_1Permissible.md) \* &gt; | [**getPermissionSubscriptions**](#function-getpermissionsubscriptions) (std::string permission) const = 0<br> |
| virtual std::unordered\_set&lt; [**Permission**](classendstone_1_1Permission.md) \* &gt; | [**getPermissions**](#function-getpermissions) () const = 0<br> |
| virtual [**Plugin**](classendstone_1_1Plugin.md) \* | [**getPlugin**](#function-getplugin) (const std::string & name) const = 0<br> |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**getPlugins**](#function-getplugins) () const = 0<br> |
| virtual bool | [**isPluginEnabled**](#function-ispluginenabled-12) (const std::string & name) const = 0<br> |
| virtual bool | [**isPluginEnabled**](#function-ispluginenabled-22) ([**Plugin**](classendstone_1_1Plugin.md) \* plugin) const = 0<br> |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**loadPlugins**](#function-loadplugins) (const std::string & directory) = 0<br> |
|  [**PluginManager**](classendstone_1_1PluginManager.md) & | [**operator=**](#function-operator) ([**PluginManager**](classendstone_1_1PluginManager.md) const &) = delete<br> |
| virtual void | [**recalculatePermissionDefaults**](#function-recalculatepermissiondefaults) ([**Permission**](classendstone_1_1Permission.md) & perm) = 0<br> |
| virtual void | [**registerEvent**](#function-registerevent) (std::string event, std::function&lt; void([**Event**](classendstone_1_1Event.md) &)&gt; executor, EventPriority priority, [**Plugin**](classendstone_1_1Plugin.md) & plugin, bool ignore\_cancelled) = 0<br> |
| virtual void | [**registerLoader**](#function-registerloader) (std::unique\_ptr&lt; [**PluginLoader**](classendstone_1_1PluginLoader.md) &gt; loader) = 0<br> |
| virtual void | [**removePermission**](#function-removepermission-12) ([**Permission**](classendstone_1_1Permission.md) & perm) = 0<br> |
| virtual void | [**removePermission**](#function-removepermission-22) (std::string name) = 0<br> |
| virtual void | [**subscribeToDefaultPerms**](#function-subscribetodefaultperms) (bool op, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br> |
| virtual void | [**subscribeToPermission**](#function-subscribetopermission) (std::string permission, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br> |
| virtual void | [**unsubscribeFromDefaultPerms**](#function-unsubscribefromdefaultperms) (bool op, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br> |
| virtual void | [**unsubscribeFromPermission**](#function-unsubscribefrompermission) (std::string permission, [**Permissible**](classendstone_1_1Permissible.md) & permissible) = 0<br> |
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
    PluginManager const &
) = delete
```




<hr>



### function addPermission 


```C++
virtual Permission * endstone::PluginManager::addPermission (
    std::unique_ptr< Permission > perm
) = 0
```



Adds a [**Permission**](classendstone_1_1Permission.md) to this plugin manager.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to add 



**Returns:**

[**Permission**](classendstone_1_1Permission.md), or nullptr if a permission is already defined with the given name of the new permission 





        

<hr>



### function callEvent 


```C++
virtual void endstone::PluginManager::callEvent (
    Event & event
) = 0
```



Calls an event which will be passed to plugins.




**Parameters:**


* `event` [**Event**](classendstone_1_1Event.md) to be called 




        

<hr>



### function clearPlugins 


```C++
virtual void endstone::PluginManager::clearPlugins () = 0
```



Disables and removes all plugins 


        

<hr>



### function disablePlugin 


```C++
virtual void endstone::PluginManager::disablePlugin (
    Plugin & plugin
) = 0
```



Disables the specified plugin Attempting to disable a plugin that is not enabled will have no effect




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to disable 




        

<hr>



### function disablePlugins 


```C++
virtual void endstone::PluginManager::disablePlugins () = 0
```



Disables all the loaded plugins 


        

<hr>



### function enablePlugin 


```C++
virtual void endstone::PluginManager::enablePlugin (
    Plugin & plugin
) const = 0
```



Enables the specified plugin Attempting to enable a plugin that is already enabled will have no effect




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to enable 




        

<hr>



### function enablePlugins 


```C++
virtual void endstone::PluginManager::enablePlugins () const = 0
```



Enable all the loaded plugins 


        

<hr>



### function getDefaultPermSubscriptions 


```C++
virtual std::unordered_set< Permissible * > endstone::PluginManager::getDefaultPermSubscriptions (
    bool op
) const = 0
```



Gets a set containing all subscribed [**Permissible**](classendstone_1_1Permissible.md)s to the given default list, by op status




**Parameters:**


* `op` Default list to query for 



**Returns:**

Set containing all subscribed permissions 





        

<hr>



### function getDefaultPermissions 


```C++
virtual std::unordered_set< Permission * > endstone::PluginManager::getDefaultPermissions (
    bool op
) const = 0
```



Gets the default permissions for the given op status




**Parameters:**


* `op` Which set of default permissions to get 



**Returns:**

The default permissions 





        

<hr>



### function getPermission 


```C++
virtual Permission * endstone::PluginManager::getPermission (
    std::string name
) const = 0
```



Gets a [**Permission**](classendstone_1_1Permission.md) from its fully qualified name




**Parameters:**


* `name` Name of the permission 



**Returns:**

[**Permission**](classendstone_1_1Permission.md), or null if none 





        

<hr>



### function getPermissionSubscriptions 


```C++
virtual std::unordered_set< Permissible * > endstone::PluginManager::getPermissionSubscriptions (
    std::string permission
) const = 0
```



Gets a set containing all subscribed Permissibles to the given permission, by name




**Parameters:**


* `permission` [**Permission**](classendstone_1_1Permission.md) to query for 



**Returns:**

Set containing all subscribed permissions 





        

<hr>



### function getPermissions 


```C++
virtual std::unordered_set< Permission * > endstone::PluginManager::getPermissions () const = 0
```



Gets a set of all registered permissions.


This set is a copy and will not be modified live.




**Returns:**

Set containing all current registered permissions 





        

<hr>



### function getPlugin 


```C++
virtual Plugin * endstone::PluginManager::getPlugin (
    const std::string & name
) const = 0
```



Checks if the given plugin is loaded and returns it when applicable. Please note that the name of the plugin is case-sensitive




**Parameters:**


* `name` Name of the plugin to check 



**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) if it exists, otherwise nullptr 





        

<hr>



### function getPlugins 


```C++
virtual std::vector< Plugin * > endstone::PluginManager::getPlugins () const = 0
```



Gets a list of all currently loaded plugins




**Returns:**

List of Plugins 





        

<hr>



### function isPluginEnabled [1/2]


```C++
virtual bool endstone::PluginManager::isPluginEnabled (
    const std::string & name
) const = 0
```



Checks if the given plugin is enabled or not Please note that the name of the plugin is case-sensitive.




**Parameters:**


* `name` Name of the plugin to check 



**Returns:**

true if the plugin is enabled, otherwise false 





        

<hr>



### function isPluginEnabled [2/2]


```C++
virtual bool endstone::PluginManager::isPluginEnabled (
    Plugin * plugin
) const = 0
```



Checks if the given plugin is enabled or not




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to check 



**Returns:**

true if the plugin is enabled, otherwise false 





        

<hr>



### function loadPlugins 


```C++
virtual std::vector< Plugin * > endstone::PluginManager::loadPlugins (
    const std::string & directory
) = 0
```



Loads the plugin contained within the specified directory




**Parameters:**


* `directory` Directory to check for plugins 



**Returns:**

A list of all plugins loaded 





        

<hr>



### function operator= 

```C++
PluginManager & endstone::PluginManager::operator= (
    PluginManager const &
) = delete
```




<hr>



### function recalculatePermissionDefaults 


```C++
virtual void endstone::PluginManager::recalculatePermissionDefaults (
    Permission & perm
) = 0
```



Recalculates the defaults for the given [**Permission**](classendstone_1_1Permission.md).


This will have no effect if the specified permission is not registered here.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to recalculate 




        

<hr>



### function registerEvent 


```C++
virtual void endstone::PluginManager::registerEvent (
    std::string event,
    std::function< void( Event &)> executor,
    EventPriority priority,
    Plugin & plugin,
    bool ignore_cancelled
) = 0
```



Registers the given event




**Parameters:**


* `event` [**Event**](classendstone_1_1Event.md) name to register 
* `executor` EventExecutor to register 
* `priority` Priority of this event 
* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to register 
* `ignore_cancelled` Do not call executor if event was already cancelled 




        

<hr>



### function registerLoader 


```C++
virtual void endstone::PluginManager::registerLoader (
    std::unique_ptr< PluginLoader > loader
) = 0
```



Registers the specified plugin loader




**Parameters:**


* `loader` [**PluginLoader**](classendstone_1_1PluginLoader.md) to register 




        

<hr>



### function removePermission [1/2]


```C++
virtual void endstone::PluginManager::removePermission (
    Permission & perm
) = 0
```



Removes a [**Permission**](classendstone_1_1Permission.md) registration from this plugin manager.


If the specified permission does not exist in this plugin manager, nothing will happen. Removing a permission registration will **not** remove the permission from any Permissibles that have it.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>



### function removePermission [2/2]


```C++
virtual void endstone::PluginManager::removePermission (
    std::string name
) = 0
```



Removes a [**Permission**](classendstone_1_1Permission.md) registration from this plugin manager.


If the specified permission does not exist in this plugin manager, nothing will happen. Removing a permission registration will **not** remove the permission from any Permissibles that have it.




**Parameters:**


* `name` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>



### function subscribeToDefaultPerms 


```C++
virtual void endstone::PluginManager::subscribeToDefaultPerms (
    bool op,
    Permissible & permissible
) = 0
```



Subscribes to the given Default permissions by operator status


If the specified defaults change in any form, the [**Permissible**](classendstone_1_1Permissible.md) will be asked to recalculate.




**Parameters:**


* `op` Default list to subscribe to 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function subscribeToPermission 


```C++
virtual void endstone::PluginManager::subscribeToPermission (
    std::string permission,
    Permissible & permissible
) = 0
```



Subscribes the given [**Permissible**](classendstone_1_1Permissible.md) for information about the requested [**Permission**](classendstone_1_1Permission.md), by name. If the specified [**Permission**](classendstone_1_1Permission.md) changes in any form, the [**Permissible**](classendstone_1_1Permissible.md) will be asked to recalculate.




**Parameters:**


* `permission` [**Permission**](classendstone_1_1Permission.md) to subscribe to 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function unsubscribeFromDefaultPerms 


```C++
virtual void endstone::PluginManager::unsubscribeFromDefaultPerms (
    bool op,
    Permissible & permissible
) = 0
```



Unsubscribes from the given Default permissions by operator status




**Parameters:**


* `op` Default list to unsubscribe from 
* `permissible` [**Permissible**](classendstone_1_1Permissible.md) subscribing 




        

<hr>



### function unsubscribeFromPermission 


```C++
virtual void endstone::PluginManager::unsubscribeFromPermission (
    std::string permission,
    Permissible & permissible
) = 0
```



Unsubscribes the given [**Permissible**](classendstone_1_1Permissible.md) for information about the requested [**Permission**](classendstone_1_1Permission.md), by name.




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

