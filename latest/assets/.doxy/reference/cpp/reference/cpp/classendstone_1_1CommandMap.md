

# Class endstone::CommandMap



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CommandMap**](classendstone_1_1CommandMap.md)



_Represents a command map that manages all commands of the_ [_**Server**_](classendstone_1_1Server.md) _._

* `#include <endstone/command/command_map.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CommandMap**](#function-commandmap-13) () = default<br> |
|   | [**CommandMap**](#function-commandmap-23) (const [**CommandMap**](classendstone_1_1CommandMap.md) &) = delete<br> |
|   | [**CommandMap**](#function-commandmap-33) ([**CommandMap**](classendstone_1_1CommandMap.md) &&) = default<br> |
| virtual void | [**clearCommands**](#function-clearcommands) () = 0<br> |
| virtual bool | [**dispatch**](#function-dispatch) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, std::string command\_line) const = 0<br>_Looks for the requested command and executes it if found._  |
| virtual std::shared\_ptr&lt; [**Command**](classendstone_1_1Command.md) &gt; | [**getCommand**](#function-getcommand) (std::string name) const = 0<br> |
|  [**CommandMap**](classendstone_1_1CommandMap.md) & | [**operator=**](#function-operator) (const [**CommandMap**](classendstone_1_1CommandMap.md) &) = delete<br> |
|  [**CommandMap**](classendstone_1_1CommandMap.md) & | [**operator=**](#function-operator_1) ([**CommandMap**](classendstone_1_1CommandMap.md) &&) = default<br> |
| virtual bool | [**registerCommand**](#function-registercommand) (std::shared\_ptr&lt; [**Command**](classendstone_1_1Command.md) &gt; command) = 0<br> |
| virtual  | [**~CommandMap**](#function-commandmap) () = default<br> |




























## Public Functions Documentation




### function CommandMap [1/3]

```C++
endstone::CommandMap::CommandMap () = default
```




<hr>



### function CommandMap [2/3]

```C++
endstone::CommandMap::CommandMap (
    const CommandMap &
) = delete
```




<hr>



### function CommandMap [3/3]

```C++
endstone::CommandMap::CommandMap (
    CommandMap &&
) = default
```




<hr>



### function clearCommands 

```C++
virtual void endstone::CommandMap::clearCommands () = 0
```



Clears all registered commands. 


        

<hr>



### function dispatch 

_Looks for the requested command and executes it if found._ 
```C++
virtual bool endstone::CommandMap::dispatch (
    CommandSender & sender,
    std::string command_line
) const = 0
```





**Parameters:**


* `sender` The command's sender 
* `command_line` command + arguments. Example: "/test abc 123" 



**Returns:**

true if execution is successful, false otherwise 





        

<hr>



### function getCommand 

```C++
virtual std::shared_ptr< Command > endstone::CommandMap::getCommand (
    std::string name
) const = 0
```



Gets the command registered to the specified name




**Parameters:**


* `name` Name of the command to retrieve 



**Returns:**

[**Command**](classendstone_1_1Command.md) with the specified name or nullptr if a command with that label doesn't exist 





        

<hr>



### function operator= 

```C++
CommandMap & endstone::CommandMap::operator= (
    const CommandMap &
) = delete
```




<hr>



### function operator= 

```C++
CommandMap & endstone::CommandMap::operator= (
    CommandMap &&
) = default
```




<hr>



### function registerCommand 

```C++
virtual bool endstone::CommandMap::registerCommand (
    std::shared_ptr< Command > command
) = 0
```



Registers a command.




**Parameters:**


* `command` the command to register 



**Returns:**

true on success, false if a command with the same name is already registered 





        

<hr>



### function ~CommandMap 

```C++
virtual endstone::CommandMap::~CommandMap () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command_map.h`

