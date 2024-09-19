

# Class endstone::CommandSender



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CommandSender**](classendstone_1_1CommandSender.md)



_Represents a command sender._ 

* `#include <endstone/command/command_sender.h>`



Inherits the following classes: [endstone::Permissible](classendstone_1_1Permissible.md)


Inherited by the following classes: [endstone::Actor](classendstone_1_1Actor.md),  [endstone::ConsoleCommandSender](classendstone_1_1ConsoleCommandSender.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CommandSender**](#function-commandsender) () = default<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual void | [**sendErrorMessage**](#function-senderrormessage-12) (const Message & message) const = 0<br>_Sends this sender a error message._  |
|  void | [**sendErrorMessage**](#function-senderrormessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual void | [**sendMessage**](#function-sendmessage-12) (const Message & message) const = 0<br>_Sends this sender a message._  |
|  void | [**sendMessage**](#function-sendmessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
|   | [**~CommandSender**](#function-commandsender) () override<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br> |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br> |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br> |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual bool | [**isOp**](classendstone_1_1Permissible.md#function-isop) () const = 0<br>_Checks if this object is a server operator._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br> |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual void | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br> |
| virtual bool | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br> |
| virtual void | [**setOp**](classendstone_1_1Permissible.md#function-setop) (bool value) = 0<br>_Sets the operator status of this object._  |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |






















































## Public Functions Documentation




### function CommandSender 

```C++
endstone::CommandSender::CommandSender () = default
```




<hr>



### function asCommandSender 

_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._
```C++
inline virtual CommandSender * endstone::CommandSender::asCommandSender () override const
```





**Returns:**

[**CommandSender**](classendstone_1_1CommandSender.md), nullptr if not a [**CommandSender**](classendstone_1_1CommandSender.md) 





        
Implements [*endstone::Permissible::asCommandSender*](classendstone_1_1Permissible.md#function-ascommandsender)


<hr>



### function asConsole 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._
```C++
inline virtual ConsoleCommandSender * endstone::CommandSender::asConsole () const
```





**Returns:**

[**CommandSender**](classendstone_1_1CommandSender.md), nullptr if not a Console 





        

<hr>



### function asPlayer 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._
```C++
inline virtual Player * endstone::CommandSender::asPlayer () const
```





**Returns:**

[**Player**](classendstone_1_1Player.md), nullptr if not a [**Player**](classendstone_1_1Player.md) 





        

<hr>



### function getName 

_Gets the name of this command sender._ 
```C++
virtual std::string endstone::CommandSender::getName () const = 0
```





**Returns:**

Name of the sender 





        

<hr>



### function getServer 

_Returns the server instance that this command is running on._ 
```C++
virtual Server & endstone::CommandSender::getServer () const = 0
```





**Returns:**

[**Server**](classendstone_1_1Server.md) instance 





        

<hr>



### function sendErrorMessage [1/2]

_Sends this sender a error message._ 
```C++
virtual void endstone::CommandSender::sendErrorMessage (
    const Message & message
) const = 0
```





**Parameters:**


* `message` Error message to be displayed 




        

<hr>



### function sendErrorMessage [2/2]

```C++
template<typename... Args>
inline void endstone::CommandSender::sendErrorMessage (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function sendMessage [1/2]

_Sends this sender a message._ 
```C++
virtual void endstone::CommandSender::sendMessage (
    const Message & message
) const = 0
```





**Parameters:**


* `message` Message to be displayed 




        

<hr>



### function sendMessage [2/2]

```C++
template<typename... Args>
inline void endstone::CommandSender::sendMessage (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function ~CommandSender 

```C++
endstone::CommandSender::~CommandSender () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command_sender.h`

