

# Class endstone::CommandSender



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CommandSender**](classendstone_1_1CommandSender.md)



_Represents a command sender._ 

* `#include <endstone/command/command_sender.h>`



Inherits the following classes: [endstone::Permissible](classendstone_1_1Permissible.md)


Inherited by the following classes: [endstone::Actor](classendstone_1_1Actor.md),  [endstone::CommandSenderWrapper](classendstone_1_1CommandSenderWrapper.md),  [endstone::ConsoleCommandSender](classendstone_1_1ConsoleCommandSender.md),  [endstone::ProxiedCommandSender](classendstone_1_1ProxiedCommandSender.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](#function-asactor) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](#function-asmob) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual [**ProxiedCommandSender**](classendstone_1_1ProxiedCommandSender.md) \* | [**asProxiedCommandSender**](#function-asproxiedcommandsender) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ProxiedCommandSender**_](classendstone_1_1ProxiedCommandSender.md) _._ |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](#function-senderrormessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Sends this sender a error message._  |
|  [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](#function-senderrormessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendMessage**](#function-sendmessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Sends this sender a message._  |
|  [**void**](classendstone_1_1Vector.md) | [**sendMessage**](#function-sendmessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Vector.md) std::string & name, [**bool**](classendstone_1_1Vector.md) value) = 0<br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) ([**const**](classendstone_1_1Vector.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isOp**](classendstone_1_1Permissible.md#function-isop) () const = 0<br>_Checks if this object is a server operator._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) ([**const**](classendstone_1_1Vector.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br> |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setOp**](classendstone_1_1Permissible.md#function-setop) ([**bool**](classendstone_1_1Vector.md) value) = 0<br>_Sets the operator status of this object._  |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |






















































## Public Functions Documentation




### function asActor 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._
```C++
inline virtual Actor * endstone::CommandSender::asActor () const
```





**Returns:**

[**Actor**](classendstone_1_1Actor.md), nullptr if not an [**Actor**](classendstone_1_1Actor.md) 





        

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



### function asMob 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._
```C++
inline virtual Mob * endstone::CommandSender::asMob () const
```





**Returns:**

[**Mob**](classendstone_1_1Mob.md), nullptr if not an [**Mob**](classendstone_1_1Mob.md) 





        

<hr>



### function asPlayer 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._
```C++
inline virtual Player * endstone::CommandSender::asPlayer () const
```





**Returns:**

[**Player**](classendstone_1_1Player.md), nullptr if not a [**Player**](classendstone_1_1Player.md) 





        

<hr>



### function asProxiedCommandSender 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ProxiedCommandSender**_](classendstone_1_1ProxiedCommandSender.md) _._
```C++
inline virtual ProxiedCommandSender * endstone::CommandSender::asProxiedCommandSender () const
```





**Returns:**

[**ProxiedCommandSender**](classendstone_1_1ProxiedCommandSender.md), nullptr if not a [**ProxiedCommandSender**](classendstone_1_1ProxiedCommandSender.md) 





        

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

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command_sender.h`

