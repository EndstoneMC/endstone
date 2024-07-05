

# Class endstone::Command



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Command**](classendstone_1_1Command.md)



_Represents a_ [_**Command**_](classendstone_1_1Command.md) _, which executes various tasks upon user input._

* `#include <endstone/command/command.h>`





Inherited by the following classes: [endstone::PluginCommand](classendstone_1_1PluginCommand.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Command**](#function-command) (std::string name, std::string description="", std::vector&lt; std::string &gt; usages={}, std::vector&lt; std::string &gt; aliases={}, std::vector&lt; std::string &gt; permissions={}) <br> |
| virtual [**PluginCommand**](classendstone_1_1PluginCommand.md) \* | [**asPluginCommand**](#function-asplugincommand) () <br> |
| virtual bool | [**execute**](#function-execute) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, const std::vector&lt; std::string &gt; & args) const<br> |
|  std::vector&lt; std::string &gt; | [**getAliases**](#function-getaliases) () const<br> |
|  std::string | [**getDescription**](#function-getdescription) () const<br> |
|  std::string | [**getName**](#function-getname) () const<br> |
|  std::vector&lt; std::string &gt; | [**getPermissions**](#function-getpermissions) () const<br> |
|  std::vector&lt; std::string &gt; | [**getUsages**](#function-getusages) () const<br> |
|  bool | [**isRegistered**](#function-isregistered) () const<br> |
|  bool | [**registerTo**](#function-registerto) ([**CommandMap**](classendstone_1_1CommandMap.md) & command\_map) <br> |
|  void | [**setAliases**](#function-setaliases) (Alias... aliases) <br> |
|  void | [**setDescription**](#function-setdescription) (std::string description) <br> |
|  void | [**setName**](#function-setname) (std::string name) <br> |
|  void | [**setPermissions**](#function-setpermissions) (Permission... permissions) <br> |
|  void | [**setUsages**](#function-setusages) (Usage... usages) <br> |
|  bool | [**testPermission**](#function-testpermission) (const [**CommandSender**](classendstone_1_1CommandSender.md) & target) const<br> |
|  bool | [**testPermissionSilently**](#function-testpermissionsilently) (const [**CommandSender**](classendstone_1_1CommandSender.md) & target) const<br> |
|  bool | [**unregisterFrom**](#function-unregisterfrom) ([**CommandMap**](classendstone_1_1CommandMap.md) & command\_map) <br> |
| virtual  | [**~Command**](#function-command) () = default<br> |




























## Public Functions Documentation




### function Command 

```C++
inline explicit endstone::Command::Command (
    std::string name,
    std::string description="",
    std::vector< std::string > usages={},
    std::vector< std::string > aliases={},
    std::vector< std::string > permissions={}
) 
```




<hr>



### function asPluginCommand 

```C++
inline virtual PluginCommand * endstone::Command::asPluginCommand () 
```




<hr>



### function execute 


```C++
inline virtual bool endstone::Command::execute (
    CommandSender & sender,
    const std::vector< std::string > & args
) const
```



Executes the command, returning its success




**Parameters:**


* `sender` Source of the command 
* `args` Arguments passed to the command 



**Returns:**

true if the execution was successful, otherwise false 





        

<hr>



### function getAliases 


```C++
inline std::vector< std::string > endstone::Command::getAliases () const
```



Returns a list of aliases of this command




**Returns:**

List of aliases 





        

<hr>



### function getDescription 


```C++
inline std::string endstone::Command::getDescription () const
```



Gets a brief description of this command




**Returns:**

Description of this command 





        

<hr>



### function getName 


```C++
inline std::string endstone::Command::getName () const
```



Returns the name of this command




**Returns:**

Name of this command 





        

<hr>



### function getPermissions 


```C++
inline std::vector< std::string > endstone::Command::getPermissions () const
```



Gets the permissions required by users to be able to perform this command




**Returns:**

List of permission names, or empty if none 





        

<hr>



### function getUsages 


```C++
inline std::vector< std::string > endstone::Command::getUsages () const
```



Returns a list of usages of this command




**Returns:**

List of usages 





        

<hr>



### function isRegistered 


```C++
inline bool endstone::Command::isRegistered () const
```



Returns the current registered state of this command




**Returns:**

true if this command is currently registered false otherwise 





        

<hr>



### function registerTo 


```C++
inline bool endstone::Command::registerTo (
    CommandMap & command_map
) 
```



Registers this command to a [**CommandMap**](classendstone_1_1CommandMap.md).




**Parameters:**


* `command_map` the [**CommandMap**](classendstone_1_1CommandMap.md) to register to 



**Returns:**

true if the registration was successful, false otherwise 





        

<hr>



### function setAliases 


```C++
template<typename... Alias>
inline void endstone::Command::setAliases (
    Alias... aliases
) 
```



Sets the list of aliases to request on registration for this command.




**Parameters:**


* `aliases` aliases to register to this command 




        

<hr>



### function setDescription 


```C++
inline void endstone::Command::setDescription (
    std::string description
) 
```



Sets a brief description of this command.




**Parameters:**


* `description` new command description 




        

<hr>



### function setName 


```C++
inline void endstone::Command::setName (
    std::string name
) 
```



Sets the name of this command.


May only be used before registering the command.




**Parameters:**


* `name` New command name 




        

<hr>



### function setPermissions 


```C++
template<typename... Permission>
inline void endstone::Command::setPermissions (
    Permission... permissions
) 
```



Sets the permissions required by users to be able to perform this command




**Parameters:**


* `permissions` List of permission names 




        

<hr>



### function setUsages 


```C++
template<typename... Usage>
inline void endstone::Command::setUsages (
    Usage... usages
) 
```



Sets the usages of this command




**Parameters:**


* `usages` List of usages 




        

<hr>



### function testPermission 


```C++
inline bool endstone::Command::testPermission (
    const CommandSender & target
) const
```



Tests the given [**CommandSender**](classendstone_1_1CommandSender.md) to see if they can perform this command. If they do not have permission, they will be informed that they cannot do this.




**Parameters:**


* `target` User to test 



**Returns:**

true if they can use it, otherwise false 





        

<hr>



### function testPermissionSilently 


```C++
inline bool endstone::Command::testPermissionSilently (
    const CommandSender & target
) const
```



Tests the given [**CommandSender**](classendstone_1_1CommandSender.md) to see if they can perform this command. No error is sent to the sender.




**Parameters:**


* `target` User to test 



**Returns:**

true if they can use it, otherwise false 





        

<hr>



### function unregisterFrom 


```C++
inline bool endstone::Command::unregisterFrom (
    CommandMap & command_map
) 
```



Unregisters this command from a [**CommandMap**](classendstone_1_1CommandMap.md)




**Parameters:**


* `command_map` the [**CommandMap**](classendstone_1_1CommandMap.md) to unregister from 



**Returns:**

true if the unregistration was successful, false otherwise 





        

<hr>



### function ~Command 

```C++
virtual endstone::Command::~Command () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command.h`

