

# Class endstone::CommandSenderWrapper



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CommandSenderWrapper**](classendstone_1_1CommandSenderWrapper.md)



_Represents a wrapper that forwards commands to the wrapped_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _and captures its output._

* `#include <endstone/command/command_sender_wrapper.h>`



Inherits the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void(const Message &)&gt; | [**Callback**](#typedef-callback)  <br> |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CommandSenderWrapper**](#function-commandsenderwrapper) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, Callback on\_message={}, Callback on\_error={}) <br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) override<br>_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._ |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) override<br>_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._ |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](#function-asactor) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](#function-asconsole) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ConsoleCommandSender**_](classendstone_1_1ConsoleCommandSender.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](#function-asplayer) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](#function-geteffectivepermissions) () override const<br>_Gets a set containing all the permissions currently in effect by this object._  |
| virtual std::string | [**getName**](#function-getname) () override const<br>_Gets the name of this command sender._  |
| virtual PermissionLevel | [**getPermissionLevel**](#function-getpermissionlevel) () override const<br>_Gets the permission level of this object._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () override const<br>_Returns the server instance that this command is running on._  |
| virtual bool | [**hasPermission**](#function-haspermission-12) (std::string name) override const<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**hasPermission**](#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) override const<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**isPermissionSet**](#function-ispermissionset-12) (std::string name) override const<br>_Checks if this object contains an override for the specified permission, by fully qualified name._  |
| virtual bool | [**isPermissionSet**](#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) override const<br>_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
| virtual void | [**recalculatePermissions**](#function-recalculatepermissions) () override<br>_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._  |
| virtual Result&lt; void &gt; | [**removeAttachment**](#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) override<br>_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._ |
| virtual void | [**sendErrorMessage**](#function-senderrormessage) (const Message & message) override const<br>_Sends this sender a error message._  |
| virtual void | [**sendMessage**](#function-sendmessage) (const Message & message) override const<br>_Sends this sender a message._  |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1CommandSender.md#function-asactor) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**BlockCommandSender**](classendstone_1_1BlockCommandSender.md) \* | [**asBlock**](classendstone_1_1CommandSender.md#function-asblock) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**BlockCommandSender**_](classendstone_1_1BlockCommandSender.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ConsoleCommandSender**_](classendstone_1_1ConsoleCommandSender.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) (const Message & message) const = 0<br>_Sends this sender a error message._  |
|  void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) (const Message & message) const = 0<br>_Sends this sender a message._  |
|  void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br>_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._ |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br>_Gets a set containing all the permissions currently in effect by this object._  |
| virtual PermissionLevel | [**getPermissionLevel**](classendstone_1_1Permissible.md#function-getpermissionlevel) () const = 0<br>_Gets the permission level of this object._  |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br>_Checks if this object contains an override for the specified permission, by fully qualified name._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
| virtual void | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br>_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._  |
| virtual Result&lt; void &gt; | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br>_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._ |
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

_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._
```C++
inline virtual PermissionAttachment * endstone::CommandSenderWrapper::addAttachment (
    Plugin & plugin,
    const std::string & name,
    bool value
) override
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 
* `name` Name of the permission to attach 
* `value` Value of the permission 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        
Implements [*endstone::Permissible::addAttachment*](classendstone_1_1Permissible.md#function-addattachment-12)


<hr>



### function addAttachment [2/2]

_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._
```C++
inline virtual PermissionAttachment * endstone::CommandSenderWrapper::addAttachment (
    Plugin & plugin
) override
```





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



### function asConsole 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ConsoleCommandSender**_](classendstone_1_1ConsoleCommandSender.md) _._
```C++
inline virtual ConsoleCommandSender * endstone::CommandSenderWrapper::asConsole () override const
```





**Returns:**

[**CommandSender**](classendstone_1_1CommandSender.md), nullptr if not a [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) 





        
Implements [*endstone::CommandSender::asConsole*](classendstone_1_1CommandSender.md#function-asconsole)


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

_Gets a set containing all the permissions currently in effect by this object._ 
```C++
inline virtual std::unordered_set< PermissionAttachmentInfo * > endstone::CommandSenderWrapper::getEffectivePermissions () override const
```





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



### function getPermissionLevel 

_Gets the permission level of this object._ 
```C++
inline virtual PermissionLevel endstone::CommandSenderWrapper::getPermissionLevel () override const
```





**Returns:**

The permission level 





        
Implements [*endstone::Permissible::getPermissionLevel*](classendstone_1_1Permissible.md#function-getpermissionlevel)


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

_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._ 
```C++
inline virtual bool endstone::CommandSenderWrapper::hasPermission (
    std::string name
) override const
```





**Parameters:**


* `name` Name of the permission 



**Returns:**

Value of the permission 





        
Implements [*endstone::Permissible::hasPermission*](classendstone_1_1Permissible.md#function-haspermission-12)


<hr>



### function hasPermission [2/2]

_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._ 
```C++
inline virtual bool endstone::CommandSenderWrapper::hasPermission (
    const Permission & perm
) override const
```





**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to get 



**Returns:**

Value of the permission 





        
Implements [*endstone::Permissible::hasPermission*](classendstone_1_1Permissible.md#function-haspermission-22)


<hr>



### function isPermissionSet [1/2]

_Checks if this object contains an override for the specified permission, by fully qualified name._ 
```C++
inline virtual bool endstone::CommandSenderWrapper::isPermissionSet (
    std::string name
) override const
```





**Parameters:**


* `name` Name of the permission 



**Returns:**

true if the permission is set, otherwise false 





        
Implements [*endstone::Permissible::isPermissionSet*](classendstone_1_1Permissible.md#function-ispermissionset-12)


<hr>



### function isPermissionSet [2/2]

_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._
```C++
inline virtual bool endstone::CommandSenderWrapper::isPermissionSet (
    const Permission & perm
) override const
```





**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to check 



**Returns:**

true if the permission is set, otherwise false 





        
Implements [*endstone::Permissible::isPermissionSet*](classendstone_1_1Permissible.md#function-ispermissionset-22)


<hr>



### function recalculatePermissions 

_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._ 
```C++
inline virtual void endstone::CommandSenderWrapper::recalculatePermissions () override
```



Implements [*endstone::Permissible::recalculatePermissions*](classendstone_1_1Permissible.md#function-recalculatepermissions)


<hr>



### function removeAttachment 

_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._
```C++
inline virtual Result< void > endstone::CommandSenderWrapper::removeAttachment (
    PermissionAttachment & attachment
) override
```





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


* `message` Error message to be displayed 




        
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

------------------------------
The documentation for this class was generated from the following file `include/endstone/command/command_sender_wrapper.h`

