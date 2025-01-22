

# Class endstone::CommandSenderWrapper



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CommandSenderWrapper**](classendstone_1_1CommandSenderWrapper.md)



_Represents a wrapper that forwards commands to the wrapped_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _and captures its output._

* `#include <endstone/command/command_sender_wrapper.h>`



Inherits the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; [**void**](classendstone_1_1Vector.md)([**const**](classendstone_1_1Vector.md) Message &)&gt; | [**Callback**](#typedef-callback)  <br> |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CommandSenderWrapper**](#function-commandsenderwrapper) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, Callback on\_message={}, Callback on\_error={}) <br> |
| virtual Result&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* &gt; | [**addAttachment**](#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Vector.md) std::string & name, [**bool**](classendstone_1_1Vector.md) value) override<br> |
| virtual Result&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* &gt; | [**addAttachment**](#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) override<br> |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](#function-asactor) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](#function-asconsole) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](#function-asmob) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](#function-asplayer) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](#function-geteffectivepermissions) () override const<br> |
| virtual std::string | [**getName**](#function-getname) () override const<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () override const<br>_Returns the server instance that this command is running on._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasPermission**](#function-haspermission-12) (std::string name) override const<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasPermission**](#function-haspermission-22) ([**const**](classendstone_1_1Vector.md) [**Permission**](classendstone_1_1Permission.md) & perm) override const<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isOp**](#function-isop) () override const<br>_Checks if this object is a server operator._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPermissionSet**](#function-ispermissionset-12) (std::string name) override const<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPermissionSet**](#function-ispermissionset-22) ([**const**](classendstone_1_1Vector.md) [**Permission**](classendstone_1_1Permission.md) & perm) override const<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**recalculatePermissions**](#function-recalculatepermissions) () override<br> |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**removeAttachment**](#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) override<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](#function-senderrormessage) ([**const**](classendstone_1_1Vector.md) Message & message) override const<br>_Sends this sender a error message._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendMessage**](#function-sendmessage) ([**const**](classendstone_1_1Vector.md) Message & message) override const<br>_Sends this sender a message._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setOp**](#function-setop) ([**bool**](classendstone_1_1Vector.md) value) override<br>_Sets the operator status of this object._  |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1CommandSender.md#function-asactor) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](classendstone_1_1CommandSender.md#function-asmob) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Sends this sender a error message._  |
|  [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Sends this sender a message._  |
|  [**void**](classendstone_1_1Vector.md) | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual Result&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* &gt; | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Vector.md) std::string & name, [**bool**](classendstone_1_1Vector.md) value) = 0<br> |
| virtual Result&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* &gt; | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
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
















































































## Public Types Documentation




### typedef Callback 

```C++
using endstone::CommandSenderWrapper::Callback =  std::function<void(const Message &)>;
```




<hr>
## Public Functions Documentation




### function CommandSenderWrapper 

```C++
inline explicit endstone::CommandSenderWrapper::CommandSenderWrapper (
    CommandSender & sender,
    Callback on_message={},
    Callback on_error={}
) 
```




<hr>



### function addAttachment [1/2]

```C++
inline virtual Result< PermissionAttachment * > endstone::CommandSenderWrapper::addAttachment (
    Plugin & plugin,
    const std::string & name,
    bool value
) override
```



Adds a new [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) with a single permission by name and value




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 
* `name` Name of the permission to attach 
* `value` Value of the permission 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        
Implements [*endstone::Permissible::addAttachment*](classendstone_1_1Permissible.md#function-addattachment-12)


<hr>



### function addAttachment [2/2]

```C++
inline virtual Result< PermissionAttachment * > endstone::CommandSenderWrapper::addAttachment (
    Plugin & plugin
) override
```



Adds a new empty [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) to this object




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        
Implements [*endstone::Permissible::addAttachment*](classendstone_1_1Permissible.md#function-addattachment-22)


<hr>



### function asActor 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._
```C++
inline virtual Actor * endstone::CommandSenderWrapper::asActor () override const
```





**Returns:**

[**Actor**](classendstone_1_1Actor.md), nullptr if not an [**Actor**](classendstone_1_1Actor.md) 





        
Implements [*endstone::CommandSender::asActor*](classendstone_1_1CommandSender.md#function-asactor)


<hr>



### function asCommandSender 

_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._
```C++
inline virtual CommandSender * endstone::CommandSenderWrapper::asCommandSender () override const
```





**Returns:**

[**CommandSender**](classendstone_1_1CommandSender.md), nullptr if not a [**CommandSender**](classendstone_1_1CommandSender.md) 





        
Implements [*endstone::CommandSender::asCommandSender*](classendstone_1_1CommandSender.md#function-ascommandsender)


<hr>



### function asConsole 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._
```C++
inline virtual ConsoleCommandSender * endstone::CommandSenderWrapper::asConsole () override const
```





**Returns:**

[**CommandSender**](classendstone_1_1CommandSender.md), nullptr if not a Console 





        
Implements [*endstone::CommandSender::asConsole*](classendstone_1_1CommandSender.md#function-asconsole)


<hr>



### function asMob 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._
```C++
inline virtual Mob * endstone::CommandSenderWrapper::asMob () override const
```





**Returns:**

[**Mob**](classendstone_1_1Mob.md), nullptr if not an [**Mob**](classendstone_1_1Mob.md) 





        
Implements [*endstone::CommandSender::asMob*](classendstone_1_1CommandSender.md#function-asmob)


<hr>



### function asPlayer 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._
```C++
inline virtual Player * endstone::CommandSenderWrapper::asPlayer () override const
```





**Returns:**

[**Player**](classendstone_1_1Player.md), nullptr if not a [**Player**](classendstone_1_1Player.md) 





        
Implements [*endstone::CommandSender::asPlayer*](classendstone_1_1CommandSender.md#function-asplayer)


<hr>



### function getEffectivePermissions 

```C++
inline virtual std::unordered_set< PermissionAttachmentInfo * > endstone::CommandSenderWrapper::getEffectivePermissions () override const
```



Gets a set containing all the permissions currently in effect by this object




**Returns:**

Set of currently effective permissions 





        
Implements [*endstone::Permissible::getEffectivePermissions*](classendstone_1_1Permissible.md#function-geteffectivepermissions)


<hr>



### function getName 

_Gets the name of this command sender._ 
```C++
inline virtual std::string endstone::CommandSenderWrapper::getName () override const
```





**Returns:**

Name of the sender 





        
Implements [*endstone::CommandSender::getName*](classendstone_1_1CommandSender.md#function-getname)


<hr>



### function getServer 

_Returns the server instance that this command is running on._ 
```C++
inline virtual Server & endstone::CommandSenderWrapper::getServer () override const
```





**Returns:**

[**Server**](classendstone_1_1Server.md) instance 





        
Implements [*endstone::CommandSender::getServer*](classendstone_1_1CommandSender.md#function-getserver)


<hr>



### function hasPermission [1/2]

```C++
inline virtual bool endstone::CommandSenderWrapper::hasPermission (
    std::string name
) override const
```



Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned.




**Parameters:**


* `name` Name of the permission 



**Returns:**

Value of the permission 





        
Implements [*endstone::Permissible::hasPermission*](classendstone_1_1Permissible.md#function-haspermission-12)


<hr>



### function hasPermission [2/2]

```C++
inline virtual bool endstone::CommandSenderWrapper::hasPermission (
    const  Permission & perm
) override const
```



Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to get 



**Returns:**

Value of the permission 





        
Implements [*endstone::Permissible::hasPermission*](classendstone_1_1Permissible.md#function-haspermission-22)


<hr>



### function isOp 

_Checks if this object is a server operator._ 
```C++
inline virtual bool endstone::CommandSenderWrapper::isOp () override const
```





**Returns:**

true if this is an operator, otherwise false 





        
Implements [*endstone::Permissible::isOp*](classendstone_1_1Permissible.md#function-isop)


<hr>



### function isPermissionSet [1/2]

```C++
inline virtual bool endstone::CommandSenderWrapper::isPermissionSet (
    std::string name
) override const
```



Checks if this object contains an override for the specified permission, by fully qualified name




**Parameters:**


* `name` Name of the permission 



**Returns:**

true if the permission is set, otherwise false 





        
Implements [*endstone::Permissible::isPermissionSet*](classendstone_1_1Permissible.md#function-ispermissionset-12)


<hr>



### function isPermissionSet [2/2]

```C++
inline virtual bool endstone::CommandSenderWrapper::isPermissionSet (
    const  Permission & perm
) override const
```



Checks if this object contains an override for the specified [**Permission**](classendstone_1_1Permission.md)




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to check 



**Returns:**

true if the permission is set, otherwise false 





        
Implements [*endstone::Permissible::isPermissionSet*](classendstone_1_1Permissible.md#function-ispermissionset-22)


<hr>



### function recalculatePermissions 

```C++
inline virtual void endstone::CommandSenderWrapper::recalculatePermissions () override
```



Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin. 


        
Implements [*endstone::Permissible::recalculatePermissions*](classendstone_1_1Permissible.md#function-recalculatepermissions)


<hr>



### function removeAttachment 

```C++
inline virtual Result< void > endstone::CommandSenderWrapper::removeAttachment (
    PermissionAttachment & attachment
) override
```



Removes the given [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) from this object




**Parameters:**


* `attachment` Attachment to remove 



**Returns:**

true if the specified attachment was removed successfully, false when it isn't part of this object 





        
Implements [*endstone::Permissible::removeAttachment*](classendstone_1_1Permissible.md#function-removeattachment)


<hr>



### function sendErrorMessage 

_Sends this sender a error message._ 
```C++
inline virtual void endstone::CommandSenderWrapper::sendErrorMessage (
    const Message & message
) override const
```





**Parameters:**


* `message` [**Error**](classendstone_1_1Error.md) message to be displayed 




        
Implements [*endstone::CommandSender::sendErrorMessage*](classendstone_1_1CommandSender.md#function-senderrormessage-12)


<hr>



### function sendMessage 

_Sends this sender a message._ 
```C++
inline virtual void endstone::CommandSenderWrapper::sendMessage (
    const Message & message
) override const
```





**Parameters:**


* `message` Message to be displayed 




        
Implements [*endstone::CommandSender::sendMessage*](classendstone_1_1CommandSender.md#function-sendmessage-12)


<hr>



### function setOp 

_Sets the operator status of this object._ 
```C++
inline virtual void endstone::CommandSenderWrapper::setOp (
    bool value
) override
```





**Parameters:**


* `value` New operator value 




        
Implements [*endstone::Permissible::setOp*](classendstone_1_1Permissible.md#function-setop)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command_sender_wrapper.h`

