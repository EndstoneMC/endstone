

# Class endstone::PluginCommand



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginCommand**](classendstone_1_1PluginCommand.md)



_Represents a_ [_**Command**_](classendstone_1_1Command.md) _belonging to a_[_**Plugin**_](classendstone_1_1Plugin.md) _._

* `#include <endstone/command/plugin_command.h>`



Inherits the following classes: [endstone::Command](classendstone_1_1Command.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginCommand**](#function-plugincommand) (const [**Command**](classendstone_1_1Command.md) & command, [**Plugin**](classendstone_1_1Plugin.md) & owner) <br> |
| virtual [**PluginCommand**](classendstone_1_1PluginCommand.md) \* | [**asPluginCommand**](#function-asplugincommand) () override<br> |
| virtual bool | [**execute**](#function-execute) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, const std::vector&lt; std::string &gt; & args) override const<br> |
| virtual [**CommandExecutor**](classendstone_1_1CommandExecutor.md) & | [**getExecutor**](#function-getexecutor) () const<br> |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br> |
| virtual void | [**setExecutor**](#function-setexecutor) (std::shared\_ptr&lt; [**CommandExecutor**](classendstone_1_1CommandExecutor.md) &gt; executor) <br> |


## Public Functions inherited from endstone::Command

See [endstone::Command](classendstone_1_1Command.md)

| Type | Name |
| ---: | :--- |
|   | [**Command**](classendstone_1_1Command.md#function-command) (std::string name, std::string description="", std::vector&lt; std::string &gt; usages={}, std::vector&lt; std::string &gt; aliases={}, std::vector&lt; std::string &gt; permissions={}) <br> |
| virtual [**PluginCommand**](classendstone_1_1PluginCommand.md) \* | [**asPluginCommand**](classendstone_1_1Command.md#function-asplugincommand) () <br> |
| virtual bool | [**execute**](classendstone_1_1Command.md#function-execute) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, const std::vector&lt; std::string &gt; & args) const<br> |
|  std::vector&lt; std::string &gt; | [**getAliases**](classendstone_1_1Command.md#function-getaliases) () const<br> |
|  std::string | [**getDescription**](classendstone_1_1Command.md#function-getdescription) () const<br> |
|  std::string | [**getName**](classendstone_1_1Command.md#function-getname) () const<br> |
|  std::vector&lt; std::string &gt; | [**getPermissions**](classendstone_1_1Command.md#function-getpermissions) () const<br> |
|  std::vector&lt; std::string &gt; | [**getUsages**](classendstone_1_1Command.md#function-getusages) () const<br> |
|  bool | [**isRegistered**](classendstone_1_1Command.md#function-isregistered) () const<br> |
|  bool | [**registerTo**](classendstone_1_1Command.md#function-registerto) ([**CommandMap**](classendstone_1_1CommandMap.md) & command\_map) <br> |
|  void | [**setAliases**](classendstone_1_1Command.md#function-setaliases) (Alias... aliases) <br> |
|  void | [**setDescription**](classendstone_1_1Command.md#function-setdescription) (std::string description) <br> |
|  void | [**setName**](classendstone_1_1Command.md#function-setname) (std::string name) <br> |
|  void | [**setPermissions**](classendstone_1_1Command.md#function-setpermissions) (Permission... permissions) <br> |
|  void | [**setUsages**](classendstone_1_1Command.md#function-setusages) (Usage... usages) <br> |
|  bool | [**testPermission**](classendstone_1_1Command.md#function-testpermission) (const [**CommandSender**](classendstone_1_1CommandSender.md) & target) const<br> |
|  bool | [**testPermissionSilently**](classendstone_1_1Command.md#function-testpermissionsilently) (const [**CommandSender**](classendstone_1_1CommandSender.md) & target) const<br> |
|  bool | [**unregisterFrom**](classendstone_1_1Command.md#function-unregisterfrom) ([**CommandMap**](classendstone_1_1CommandMap.md) & command\_map) <br> |
| virtual  | [**~Command**](classendstone_1_1Command.md#function-command) () = default<br> |






















































## Public Functions Documentation




### function PluginCommand 

```C++
inline endstone::PluginCommand::PluginCommand (
    const Command & command,
    Plugin & owner
) 
```




<hr>



### function asPluginCommand 

```C++
inline virtual PluginCommand * endstone::PluginCommand::asPluginCommand () override
```



Implements [*endstone::Command::asPluginCommand*](classendstone_1_1Command.md#function-asplugincommand)


<hr>



### function execute 


```C++
inline virtual bool endstone::PluginCommand::execute (
    CommandSender & sender,
    const std::vector< std::string > & args
) override const
```



Executes the command, returning its success




**Parameters:**


* `sender` Source of the command 
* `args` Arguments passed to the command 



**Returns:**

true if the execution was successful, otherwise false 





        
Implements [*endstone::Command::execute*](classendstone_1_1Command.md#function-execute)


<hr>



### function getExecutor 


```C++
inline virtual CommandExecutor & endstone::PluginCommand::getExecutor () const
```



Gets the [**CommandExecutor**](classendstone_1_1CommandExecutor.md) associated with this command




**Returns:**

[**CommandExecutor**](classendstone_1_1CommandExecutor.md) object linked to this command 





        

<hr>



### function getPlugin 


```C++
inline Plugin & endstone::PluginCommand::getPlugin () const
```



Gets the owner of this [**PluginCommand**](classendstone_1_1PluginCommand.md)




**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) that owns this command 





        

<hr>



### function setExecutor 


```C++
inline virtual void endstone::PluginCommand::setExecutor (
    std::shared_ptr< CommandExecutor > executor
) 
```



Sets the [**CommandExecutor**](classendstone_1_1CommandExecutor.md) to run when parsing this command




**Parameters:**


* `executor` New executor to run 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/plugin_command.h`

