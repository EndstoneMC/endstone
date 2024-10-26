

# Class endstone::Plugin



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Plugin**](classendstone_1_1Plugin.md)



_Represents a_ [_**Plugin**_](classendstone_1_1Plugin.md) _._

* `#include <endstone/plugin/plugin.h>`



Inherits the following classes: [endstone::CommandExecutor](classendstone_1_1CommandExecutor.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Plugin**](#function-plugin-12) () = default<br> |
|   | [**Plugin**](#function-plugin-22) (const [**Plugin**](classendstone_1_1Plugin.md) &) = delete<br> |
|  [**PluginCommand**](classendstone_1_1PluginCommand.md) \* | [**getCommand**](#function-getcommand) (std::string name) const<br>_Gets the command with the given name, specific to this plugin._  |
|  const std::filesystem::path & | [**getDataFolder**](#function-getdatafolder) () const<br>_Returns the folder that the plugin data's files are located in. The folder may not yet exist._  |
| virtual const [**PluginDescription**](classendstone_1_1PluginDescription.md) & | [**getDescription**](#function-getdescription) () const = 0<br>_Returns the details of this plugin._  |
|  [**Logger**](classendstone_1_1Logger.md) & | [**getLogger**](#function-getlogger) () const<br>_Returns the plugin logger associated with this server's logger. The returned logger automatically tags all log messages with the plugin's name._  |
|  std::string | [**getName**](#function-getname) () const<br>_Returns the name of the plugin._  |
|  [**PluginLoader**](classendstone_1_1PluginLoader.md) & | [**getPluginLoader**](#function-getpluginloader) () const<br>_Gets the associated_ [_**PluginLoader**_](classendstone_1_1PluginLoader.md) _responsible for this plugin._ |
|  [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () const<br>_Returns the_ [_**Server**_](classendstone_1_1Server.md) _instance currently running this plugin._ |
|  bool | [**isEnabled**](#function-isenabled) () const<br>_Returns a value indicating whether this plugin is currently enabled._  |
| virtual void | [**onDisable**](#function-ondisable) () <br>_Called when this plugin is disabled._  |
| virtual void | [**onEnable**](#function-onenable) () <br>_Called when this plugin is enabled._  |
| virtual void | [**onLoad**](#function-onload) () <br>_Called after a plugin is loaded but before it has been enabled._  |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**operator=**](#function-operator) (const [**Plugin**](classendstone_1_1Plugin.md) &) = delete<br> |
|  void | [**registerEvent**](#function-registerevent-12) (void(T::\*)(EventType &) func, T & instance, EventPriority priority=EventPriority::Normal, bool ignore\_cancelled=false) <br> |
|  void | [**registerEvent**](#function-registerevent-22) (std::function&lt; void(EventType &)&gt; func, EventPriority priority=EventPriority::Normal, bool ignore\_cancelled=false) <br> |
|   | [**~Plugin**](#function-plugin) () override<br> |


## Public Functions inherited from endstone::CommandExecutor

See [endstone::CommandExecutor](classendstone_1_1CommandExecutor.md)

| Type | Name |
| ---: | :--- |
| virtual bool | [**onCommand**](classendstone_1_1CommandExecutor.md#function-oncommand) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, const [**Command**](classendstone_1_1Command.md) & command, const std::vector&lt; std::string &gt; & args) <br> |
| virtual  | [**~CommandExecutor**](classendstone_1_1CommandExecutor.md#function-commandexecutor) () = default<br> |














































## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |








## Public Functions Documentation




### function Plugin [1/2]

```C++
endstone::Plugin::Plugin () = default
```




<hr>



### function Plugin [2/2]

```C++
endstone::Plugin::Plugin (
    const Plugin &
) = delete
```




<hr>



### function getCommand 

_Gets the command with the given name, specific to this plugin._ 
```C++
inline PluginCommand * endstone::Plugin::getCommand (
    std::string name
) const
```





**Parameters:**


* `name` name or alias of the command 



**Returns:**

the plugin command if found, otherwise null 





        

<hr>



### function getDataFolder 

_Returns the folder that the plugin data's files are located in. The folder may not yet exist._ 
```C++
inline const std::filesystem::path & endstone::Plugin::getDataFolder () const
```





**Returns:**

The folder 





        

<hr>



### function getDescription 

_Returns the details of this plugin._ 
```C++
virtual const PluginDescription & endstone::Plugin::getDescription () const = 0
```





**Returns:**

Details of this plugin 





        

<hr>



### function getLogger 

_Returns the plugin logger associated with this server's logger. The returned logger automatically tags all log messages with the plugin's name._ 
```C++
inline Logger & endstone::Plugin::getLogger () const
```





**Returns:**

[**Logger**](classendstone_1_1Logger.md) associated with this plugin 





        

<hr>



### function getName 

_Returns the name of the plugin._ 
```C++
inline std::string endstone::Plugin::getName () const
```



This should return the bare name of the plugin and should be used for comparison.




**Returns:**

name of the plugin 





        

<hr>



### function getPluginLoader 

_Gets the associated_ [_**PluginLoader**_](classendstone_1_1PluginLoader.md) _responsible for this plugin._
```C++
inline PluginLoader & endstone::Plugin::getPluginLoader () const
```





**Returns:**

[**PluginLoader**](classendstone_1_1PluginLoader.md) that controls this plugin 





        

<hr>



### function getServer 

_Returns the_ [_**Server**_](classendstone_1_1Server.md) _instance currently running this plugin._
```C++
inline Server & endstone::Plugin::getServer () const
```





**Returns:**

[**Server**](classendstone_1_1Server.md) running this plugin 





        

<hr>



### function isEnabled 

_Returns a value indicating whether this plugin is currently enabled._ 
```C++
inline bool endstone::Plugin::isEnabled () const
```





**Returns:**

true if this plugin is enabled, otherwise false 





        

<hr>



### function onDisable 

```C++
inline virtual void endstone::Plugin::onDisable () 
```




<hr>



### function onEnable 

```C++
inline virtual void endstone::Plugin::onEnable () 
```




<hr>



### function onLoad 

_Called after a plugin is loaded but before it has been enabled._ 
```C++
inline virtual void endstone::Plugin::onLoad () 
```



When multiple plugins are loaded, the [**onLoad()**](classendstone_1_1Plugin.md#function-onload) for all plugins is called before any [**onEnable()**](classendstone_1_1Plugin.md#function-onenable) is called. 


        

<hr>



### function operator= 

```C++
Plugin & endstone::Plugin::operator= (
    const Plugin &
) = delete
```




<hr>



### function registerEvent [1/2]

```C++
template<typename EventType, typename T>
inline void endstone::Plugin::registerEvent (
    void(T::*)(EventType &) func,
    T & instance,
    EventPriority priority=EventPriority::Normal,
    bool ignore_cancelled=false
) 
```




<hr>



### function registerEvent [2/2]

```C++
template<typename EventType>
inline void endstone::Plugin::registerEvent (
    std::function< void(EventType &)> func,
    EventPriority priority=EventPriority::Normal,
    bool ignore_cancelled=false
) 
```




<hr>



### function ~Plugin 

```C++
endstone::Plugin::~Plugin () override
```




<hr>
## Protected Functions Documentation




### function setEnabled 


```C++
inline void endstone::Plugin::setEnabled (
    bool enabled
) 
```



Sets the enabled state of this plugin




**Parameters:**


* `enabled` true if enabled, otherwise false 




        

<hr>## Friends Documentation





### friend EndstonePluginManager 

```C++
class endstone::Plugin::EndstonePluginManager (
    detail::EndstonePluginManager
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

