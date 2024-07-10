

# Class endstone::Permissible



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Permissible**](classendstone_1_1Permissible.md)



_Represents an object that may become a server operator and can be assigned permissions._ 

* `#include <endstone/permissions/permissible.h>`





Inherited by the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](#function-ascommandsender) () const = 0<br> |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](#function-geteffectivepermissions) () const = 0<br> |
| virtual bool | [**hasPermission**](#function-haspermission-12) (std::string name) const = 0<br> |
| virtual bool | [**hasPermission**](#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual bool | [**isOp**](#function-isop) () const = 0<br>_Checks if this object is a server operator._  |
| virtual bool | [**isPermissionSet**](#function-ispermissionset-12) (std::string name) const = 0<br> |
| virtual bool | [**isPermissionSet**](#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual void | [**recalculatePermissions**](#function-recalculatepermissions) () = 0<br> |
| virtual bool | [**removeAttachment**](#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br> |
| virtual void | [**setOp**](#function-setop) (bool value) = 0<br>_Sets the operator status of this object._  |
| virtual  | [**~Permissible**](#function-permissible) () = default<br> |




























## Public Functions Documentation




### function addAttachment [1/2]


```C++
virtual PermissionAttachment * endstone::Permissible::addAttachment (
    Plugin & plugin,
    const std::string & name,
    bool value
) = 0
```



Adds a new [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) with a single permission by name and value




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 
* `name` Name of the permission to attach 
* `value` Value of the permission 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        

<hr>



### function addAttachment [2/2]


```C++
virtual PermissionAttachment * endstone::Permissible::addAttachment (
    Plugin & plugin
) = 0
```



Adds a new empty [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) to this object




**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) responsible for this attachment, may not be null or disabled 



**Returns:**

The [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) that was just created 





        

<hr>



### function asCommandSender 

```C++
virtual CommandSender * endstone::Permissible::asCommandSender () const = 0
```




<hr>



### function getEffectivePermissions 


```C++
virtual std::unordered_set< PermissionAttachmentInfo * > endstone::Permissible::getEffectivePermissions () const = 0
```



Gets a set containing all of the permissions currently in effect by this object




**Returns:**

Set of currently effective permissions 





        

<hr>



### function hasPermission [1/2]


```C++
virtual bool endstone::Permissible::hasPermission (
    std::string name
) const = 0
```



Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned.




**Parameters:**


* `name` Name of the permission 



**Returns:**

Value of the permission 





        

<hr>



### function hasPermission [2/2]


```C++
virtual bool endstone::Permissible::hasPermission (
    const Permission & perm
) const = 0
```



Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to get 



**Returns:**

Value of the permission 





        

<hr>



### function isOp 

_Checks if this object is a server operator._ 
```C++
virtual bool endstone::Permissible::isOp () const = 0
```





**Returns:**

true if this is an operator, otherwise false 





        

<hr>



### function isPermissionSet [1/2]


```C++
virtual bool endstone::Permissible::isPermissionSet (
    std::string name
) const = 0
```



Checks if this object contains an override for the specified permission, by fully qualified name




**Parameters:**


* `name` Name of the permission 



**Returns:**

true if the permission is set, otherwise false 





        

<hr>



### function isPermissionSet [2/2]


```C++
virtual bool endstone::Permissible::isPermissionSet (
    const Permission & perm
) const = 0
```



Checks if this object contains an override for the specified [**Permission**](classendstone_1_1Permission.md)




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to check 



**Returns:**

true if the permission is set, otherwise false 





        

<hr>



### function recalculatePermissions 


```C++
virtual void endstone::Permissible::recalculatePermissions () = 0
```



Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin. 


        

<hr>



### function removeAttachment 


```C++
virtual bool endstone::Permissible::removeAttachment (
    PermissionAttachment & attachment
) = 0
```



Removes the given [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) from this object




**Parameters:**


* `attachment` Attachment to remove 



**Returns:**

true if the specified attachment was removed successfully, false when it isn't part of this object 





        

<hr>



### function setOp 

_Sets the operator status of this object._ 
```C++
virtual void endstone::Permissible::setOp (
    bool value
) = 0
```





**Parameters:**


* `value` New operator value 




        

<hr>



### function ~Permissible 

```C++
virtual endstone::Permissible::~Permissible () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permissible.h`

