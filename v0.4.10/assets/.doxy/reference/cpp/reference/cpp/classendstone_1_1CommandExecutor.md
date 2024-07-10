

# Class endstone::CommandExecutor



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CommandExecutor**](classendstone_1_1CommandExecutor.md)



_Represents a class which contains a single method for executing commands._ 

* `#include <endstone/command/command_executor.h>`





Inherited by the following classes: [endstone::Plugin](classendstone_1_1Plugin.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**onCommand**](#function-oncommand) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, const [**Command**](classendstone_1_1Command.md) & command, const std::vector&lt; std::string &gt; & args) <br> |
| virtual  | [**~CommandExecutor**](#function-commandexecutor) () = default<br> |




























## Public Functions Documentation




### function onCommand 


```C++
inline virtual bool endstone::CommandExecutor::onCommand (
    CommandSender & sender,
    const Command & command,
    const std::vector< std::string > & args
) 
```



Executes the given command, returning its success.




**Parameters:**


* `sender` Source of the command 
* `command` [**Command**](classendstone_1_1Command.md) which was executed 
* `args` Passed command arguments 



**Returns:**

true if the execution is successful, otherwise false 





        

<hr>



### function ~CommandExecutor 

```C++
virtual endstone::CommandExecutor::~CommandExecutor () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command_executor.h`

