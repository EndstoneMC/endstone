

# Class endstone::Permissible



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Permissible**](classendstone_1_1Permissible.md)



_Represents an object that may become a server operator and can be assigned permissions._ 

* `#include <endstone/permissions/permissible.h>`





Inherited by the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br>_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._ |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](#function-ascommandsender) () const = 0<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](#function-geteffectivepermissions) () const = 0<br>_Gets a set containing all the permissions currently in effect by this object._  |
| virtual PermissionLevel | [**getPermissionLevel**](#function-getpermissionlevel) () const = 0<br>_Gets the permission level of this object._  |
| virtual bool | [**hasPermission**](#function-haspermission-12) (std::string name) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**hasPermission**](#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**isPermissionSet**](#function-ispermissionset-12) (std::string name) const = 0<br>_Checks if this object contains an override for the specified permission, by fully qualified name._  |
| virtual bool | [**isPermissionSet**](#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
| virtual void | [**recalculatePermissions**](#function-recalculatepermissions) () = 0<br>_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._  |
| virtual Result&lt; void &gt; | [**removeAttachment**](#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br>_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._ |
| virtual  | [**~Permissible**](#function-permissible) () = default<br> |




























## Public Functions Documentation




### function addAttachment [1/2]

_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._
```C++
virtual PermissionAttachment * endstone::Permissible::addAttachment (
    Plugin & plugin,
    const std::string & name,
    bool value
) = 0
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 
* `name` Name of the permission to attach 
* `value` Value of the permission 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        

<hr>



### function addAttachment [2/2]

_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._
```C++
virtual PermissionAttachment * endstone::Permissible::addAttachment (
    Plugin & plugin
) = 0
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        

<hr>



### function asCommandSender 

_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._
```C++
virtual CommandSender * endstone::Permissible::asCommandSender () const = 0
```





**Returns:**

[**CommandSender**](classendstone_1_1CommandSender.md), nullptr if not a [**CommandSender**](classendstone_1_1CommandSender.md) 





        

<hr>



### function getEffectivePermissions 

_Gets a set containing all the permissions currently in effect by this object._ 
```C++
virtual std::unordered_set< PermissionAttachmentInfo * > endstone::Permissible::getEffectivePermissions () const = 0
```





**Returns:**

Set of currently effective permissions 





        

<hr>



### function getPermissionLevel 

_Gets the permission level of this object._ 
```C++
virtual PermissionLevel endstone::Permissible::getPermissionLevel () const = 0
```





**Returns:**

The permission level 





        

<hr>



### function hasPermission [1/2]

_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._ 
```C++
virtual bool endstone::Permissible::hasPermission (
    std::string name
) const = 0
```





**Parameters:**


* `name` Name of the permission 



**Returns:**

Value of the permission 





        

<hr>



### function hasPermission [2/2]

_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._ 
```C++
virtual bool endstone::Permissible::hasPermission (
    const Permission & perm
) const = 0
```





**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to get 



**Returns:**

Value of the permission 





        

<hr>



### function isPermissionSet [1/2]

_Checks if this object contains an override for the specified permission, by fully qualified name._ 
```C++
virtual bool endstone::Permissible::isPermissionSet (
    std::string name
) const = 0
```





**Parameters:**


* `name` Name of the permission 



**Returns:**

true if the permission is set, otherwise false 





        

<hr>



### function isPermissionSet [2/2]

_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._
```C++
virtual bool endstone::Permissible::isPermissionSet (
    const Permission & perm
) const = 0
```





**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to check 



**Returns:**

true if the permission is set, otherwise false 





        

<hr>



### function recalculatePermissions 

_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._ 
```C++
virtual void endstone::Permissible::recalculatePermissions () = 0
```




<hr>



### function removeAttachment 

_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._
```C++
virtual Result< void > endstone::Permissible::removeAttachment (
    PermissionAttachment & attachment
) = 0
```





**Parameters:**


* `attachment` Attachment to remove 



**Returns:**

true if the specified attachment was removed successfully, false when it isn't part of this object 





        

<hr>



### function ~Permissible 

```C++
virtual endstone::Permissible::~Permissible () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permissible.h`

