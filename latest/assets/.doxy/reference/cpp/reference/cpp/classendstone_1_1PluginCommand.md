

# Class endstone::PluginCommand



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginCommand**](classendstone_1_1PluginCommand.md)



_Represents a_ [_**Command**_](classendstone_1_1Command.md) _belonging to a_[_**Plugin**_](classendstone_1_1Plugin.md) _._

* `#include <endstone/command/plugin_command.h>`



Inherits the following classes: [endstone::Command](classendstone_1_1Command.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginCommand**](#function-plugincommand) ([**const**](classendstone_1_1Identifier.md) [**Command**](classendstone_1_1Command.md) & command, [**Plugin**](classendstone_1_1Plugin.md) & owner) <br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**execute**](#function-execute) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, [**const**](classendstone_1_1Identifier.md) std::vector&lt; std::string &gt; & args) override const<br>_Executes the command, returning its success._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](#function-getclasstypeid) () override const<br> |
| virtual [**CommandExecutor**](classendstone_1_1CommandExecutor.md) & | [**getExecutor**](#function-getexecutor) () const<br>_Gets the_ [_**CommandExecutor**_](classendstone_1_1CommandExecutor.md) _associated with this command._ |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br>_Gets the owner of this_ [_**PluginCommand**_](classendstone_1_1PluginCommand.md) _._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) override const<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setExecutor**](#function-setexecutor) (std::shared\_ptr&lt; [**CommandExecutor**](classendstone_1_1CommandExecutor.md) &gt; executor) <br>_Sets the_ [_**CommandExecutor**_](classendstone_1_1CommandExecutor.md) _to run when parsing this command._ |


## Public Functions inherited from endstone::Command

See [endstone::Command](classendstone_1_1Command.md)

| Type | Name |
| ---: | :--- |
|   | [**Command**](classendstone_1_1Command.md#function-command) (std::string name, std::string description="", std::vector&lt; std::string &gt; usages={}, std::vector&lt; std::string &gt; aliases={}, std::vector&lt; std::string &gt; permissions={}) <br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**execute**](classendstone_1_1Command.md#function-execute) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, [**const**](classendstone_1_1Identifier.md) std::vector&lt; std::string &gt; & args) const<br>_Executes the command, returning its success._  |
|  std::vector&lt; std::string &gt; | [**getAliases**](classendstone_1_1Command.md#function-getaliases) () const<br>_Returns a list of aliases of this command._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](classendstone_1_1Command.md#function-getclasstypeid) () override const<br> |
|  std::string | [**getDescription**](classendstone_1_1Command.md#function-getdescription) () const<br>_Gets a brief description of this command._  |
|  std::string | [**getName**](classendstone_1_1Command.md#function-getname) () const<br>_Returns the name of this command._  |
|  std::vector&lt; std::string &gt; | [**getPermissions**](classendstone_1_1Command.md#function-getpermissions) () const<br>_Gets the permissions required by users to be able to perform this command._  |
|  std::vector&lt; std::string &gt; | [**getUsages**](classendstone_1_1Command.md#function-getusages) () const<br>_Returns a list of usages of this command._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Command.md#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) override const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isRegistered**](classendstone_1_1Command.md#function-isregistered) () const<br>_Returns the current registered state of this command._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**registerTo**](classendstone_1_1Command.md#function-registerto) ([**const**](classendstone_1_1Identifier.md) [**CommandMap**](classendstone_1_1CommandMap.md) & command\_map) <br>_Registers this command to a_ [_**CommandMap**_](classendstone_1_1CommandMap.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**setAliases**](classendstone_1_1Command.md#function-setaliases) (Alias... aliases) <br>_Sets the list of aliases to request on registration for this command._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setDescription**](classendstone_1_1Command.md#function-setdescription) (std::string description) <br>_Sets a brief description of this command._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setName**](classendstone_1_1Command.md#function-setname) (std::string name) <br>_Sets the name of this command._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setPermissions**](classendstone_1_1Command.md#function-setpermissions) (Permission... permissions) <br>_Sets the permissions required by users to be able to perform this command._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setUsages**](classendstone_1_1Command.md#function-setusages) (Usage... usages) <br>_Sets the usages of this command._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**testPermission**](classendstone_1_1Command.md#function-testpermission) ([**const**](classendstone_1_1Identifier.md) [**CommandSender**](classendstone_1_1CommandSender.md) & target) const<br>_Tests the given_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _to see if they can perform this command._ |
|  [**bool**](classendstone_1_1Identifier.md) | [**testPermissionSilently**](classendstone_1_1Command.md#function-testpermissionsilently) ([**const**](classendstone_1_1Identifier.md) [**CommandSender**](classendstone_1_1CommandSender.md) & target) const<br>_Tests the given_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _to see if they can perform this command._ |
|  [**bool**](classendstone_1_1Identifier.md) | [**unregisterFrom**](classendstone_1_1Command.md#function-unregisterfrom) ([**const**](classendstone_1_1Identifier.md) [**CommandMap**](classendstone_1_1CommandMap.md) & command\_map) <br>_Unregisters this command from a_ [_**CommandMap**_](classendstone_1_1CommandMap.md) _._ |
| virtual  | [**~Command**](classendstone_1_1Command.md#function-command) () = default<br> |


## Public Functions inherited from endstone::Object

See [endstone::Object](classendstone_1_1Object.md)

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-12) () <br>_Attempts to cast this object to the given type T._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-22) () const<br>_Attempts to cast this object to the given type T._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](classendstone_1_1Object.md#function-getclasstypeid) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |
















































































## Public Functions Documentation




### function PluginCommand 

```C++
inline endstone::PluginCommand::PluginCommand (
    const  Command & command,
    Plugin & owner
) 
```




<hr>



### function execute 

_Executes the command, returning its success._ 
```C++
inline virtual bool endstone::PluginCommand::execute (
    CommandSender & sender,
    const std::vector< std::string > & args
) override const
```





**Parameters:**


* `sender` Source of the command 
* `args` Arguments passed to the command 



**Returns:**

true if the execution was successful, otherwise false 





        
Implements [*endstone::Command::execute*](classendstone_1_1Command.md#function-execute)


<hr>



### function getClassTypeId 

```C++
inline virtual const std::type_info & endstone::PluginCommand::getClassTypeId () override const
```



Implements [*endstone::Command::getClassTypeId*](classendstone_1_1Command.md#function-getclasstypeid)


<hr>



### function getExecutor 

_Gets the_ [_**CommandExecutor**_](classendstone_1_1CommandExecutor.md) _associated with this command._
```C++
inline virtual CommandExecutor & endstone::PluginCommand::getExecutor () const
```





**Returns:**

[**CommandExecutor**](classendstone_1_1CommandExecutor.md) object linked to this command 





        

<hr>



### function getPlugin 

_Gets the owner of this_ [_**PluginCommand**_](classendstone_1_1PluginCommand.md) _._
```C++
inline Plugin & endstone::PluginCommand::getPlugin () const
```





**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) that owns this command 





        

<hr>



### function isInstanceOf 

```C++
inline virtual bool endstone::PluginCommand::isInstanceOf (
    const std::type_info & target
) override const
```



Implements [*endstone::Command::isInstanceOf*](classendstone_1_1Command.md#function-isinstanceof)


<hr>



### function setExecutor 

_Sets the_ [_**CommandExecutor**_](classendstone_1_1CommandExecutor.md) _to run when parsing this command._
```C++
inline virtual void endstone::PluginCommand::setExecutor (
    std::shared_ptr< CommandExecutor > executor
) 
```





**Parameters:**


* `executor` New executor to run 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/plugin_command.h`

