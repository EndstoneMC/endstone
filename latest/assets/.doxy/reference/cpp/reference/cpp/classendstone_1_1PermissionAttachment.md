

# Class endstone::PermissionAttachment



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md)



_Holds information about a permission attachment on a_ [_**Permissible**_](classendstone_1_1Permissible.md) _object._

* `#include <endstone/permissions/permission_attachment.h>`



Inherits the following classes: std::enable_shared_from_this< PermissionAttachment >


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionAttachment**](#function-permissionattachment) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Permissible**](classendstone_1_1Permissible.md) &gt; & permissible) <br> |
|  [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Permissible**](classendstone_1_1Permissible.md) &gt; | [**getPermissible**](#function-getpermissible) () const<br>_Gets the_ [_**Permissible**_](classendstone_1_1Permissible.md) _that this is attached to._ |
|  std::unordered\_map&lt; std::string, [**bool**](classendstone_1_1Identifier.md) &gt; | [**getPermissions**](#function-getpermissions) () const<br>_Gets a copy of all set permissions and values contained within this attachment._  |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br>_Gets the plugin responsible for this attachment._  |
|  PermissionRemovedExecutor | [**getRemovalCallback**](#function-getremovalcallback) () const<br>_Gets the executor that was previously set to be called when this attachment was removed from a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._ |
|  [**bool**](classendstone_1_1Identifier.md) | [**remove**](#function-remove) () <br>_Removes this attachment from its registered_ [_**Permissible**_](classendstone_1_1Permissible.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**setPermission**](#function-setpermission-12) (std::string name, [**bool**](classendstone_1_1Identifier.md) value) <br>_Sets a permission to the given value, by its fully qualified name._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setPermission**](#function-setpermission-22) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Permission**](classendstone_1_1Permission.md) &gt; & perm, [**bool**](classendstone_1_1Identifier.md) value) <br>_Sets a permission to the given value._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setRemovalCallback**](#function-setremovalcallback) (PermissionRemovedExecutor ex) <br>_Sets an executor to be called for when this attachment is removed from a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**unsetPermission**](#function-unsetpermission-12) (std::string name) <br>_Removes the specified permission from this attachment._  |
|  [**void**](classendstone_1_1Identifier.md) | [**unsetPermission**](#function-unsetpermission-22) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Permission**](classendstone_1_1Permission.md) &gt; & perm) <br>_Removes the specified permission from this attachment._  |




























## Public Functions Documentation




### function PermissionAttachment 

```C++
inline endstone::PermissionAttachment::PermissionAttachment (
    Plugin & plugin,
    const  NotNull < Permissible > & permissible
) 
```




<hr>



### function getPermissible 

_Gets the_ [_**Permissible**_](classendstone_1_1Permissible.md) _that this is attached to._
```C++
inline Nullable < Permissible > endstone::PermissionAttachment::getPermissible () const
```





**Returns:**

[**Permissible**](classendstone_1_1Permissible.md) containing this attachment, or nullptr if it no longer exists 





        

<hr>



### function getPermissions 

_Gets a copy of all set permissions and values contained within this attachment._ 
```C++
inline std::unordered_map< std::string, bool > endstone::PermissionAttachment::getPermissions () const
```



This map may be modified but will not affect the attachment, as it is a copy.




**Returns:**

Copy of all permissions and values expressed by this attachment 





        

<hr>



### function getPlugin 

_Gets the plugin responsible for this attachment._ 
```C++
inline Plugin & endstone::PermissionAttachment::getPlugin () const
```





**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) responsible for this permission attachment 





        

<hr>



### function getRemovalCallback 

_Gets the executor that was previously set to be called when this attachment was removed from a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._
```C++
inline PermissionRemovedExecutor endstone::PermissionAttachment::getRemovalCallback () const
```



May be empty.




**Returns:**

Executor to be called when this is removed 





        

<hr>



### function remove 

_Removes this attachment from its registered_ [_**Permissible**_](classendstone_1_1Permissible.md) _._
```C++
inline bool endstone::PermissionAttachment::remove () 
```





**Returns:**

true if the attachment was removed successfully, false if it was already removed or its [**Permissible**](classendstone_1_1Permissible.md) no longer exists 





        

<hr>



### function setPermission [1/2]

_Sets a permission to the given value, by its fully qualified name._ 
```C++
inline void endstone::PermissionAttachment::setPermission (
    std::string name,
    bool value
) 
```





**Parameters:**


* `name` Name of the permission 
* `value` New value of the permission 




        

<hr>



### function setPermission [2/2]

_Sets a permission to the given value._ 
```C++
inline void endstone::PermissionAttachment::setPermission (
    const  NotNull < Permission > & perm,
    bool value
) 
```





**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to set 
* `value` New value of the permission 




        

<hr>



### function setRemovalCallback 

_Sets an executor to be called for when this attachment is removed from a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._
```C++
inline void endstone::PermissionAttachment::setRemovalCallback (
    PermissionRemovedExecutor ex
) 
```



May be empty.




**Parameters:**


* `ex` Executor to be called when this is removed 




        

<hr>



### function unsetPermission [1/2]

_Removes the specified permission from this attachment._ 
```C++
inline void endstone::PermissionAttachment::unsetPermission (
    std::string name
) 
```



If the permission does not exist in this attachment, nothing will happen.




**Parameters:**


* `name` Name of the permission to remove 




        

<hr>



### function unsetPermission [2/2]

_Removes the specified permission from this attachment._ 
```C++
inline void endstone::PermissionAttachment::unsetPermission (
    const  NotNull < Permission > & perm
) 
```



If the permission does not exist in this attachment, nothing will happen.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission_attachment.h`

