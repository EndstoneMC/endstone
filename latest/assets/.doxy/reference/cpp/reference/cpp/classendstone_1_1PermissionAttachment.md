

# Class endstone::PermissionAttachment



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md)



_Holds information about a permission attachment on a_ [_**Permissible**_](classendstone_1_1Permissible.md) _object._

* `#include <endstone/permissions/permission_attachment.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionAttachment**](#function-permissionattachment) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**Permissible**](classendstone_1_1Permissible.md) & permissible) <br> |
|  [**Permissible**](classendstone_1_1Permissible.md) & | [**getPermissible**](#function-getpermissible) () const<br>_Gets the_ [_**Permissible**_](classendstone_1_1Permissible.md) _that this is attached to._ |
|  std::unordered\_map&lt; std::string, [**bool**](classendstone_1_1Identifier.md) &gt; | [**getPermissions**](#function-getpermissions) () const<br>_Gets a copy of all set permissions and values contained within this attachment._  |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br>_Gets the plugin responsible for this attachment._  |
|  PermissionRemovedExecutor | [**getRemovalCallback**](#function-getremovalcallback) () const<br>_Gets the executor that was previously set to be called when this attachment was removed from a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._ |
|  [**bool**](classendstone_1_1Identifier.md) | [**remove**](#function-remove) () <br>_Removes this attachment from its registered_ [_**Permissible**_](classendstone_1_1Permissible.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**setPermission**](#function-setpermission-12) (std::string name, [**bool**](classendstone_1_1Identifier.md) value) <br>_Sets a permission to the given value, by its fully qualified name._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setPermission**](#function-setpermission-22) ([**Permission**](classendstone_1_1Permission.md) & perm, [**bool**](classendstone_1_1Identifier.md) value) <br>_Sets a permission to the given value._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setRemovalCallback**](#function-setremovalcallback) (PermissionRemovedExecutor ex) <br>_Sets an executor to be called for when this attachment is removed from a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**unsetPermission**](#function-unsetpermission-12) (std::string name) <br>_Removes the specified permission from this attachment._  |
|  [**void**](classendstone_1_1Identifier.md) | [**unsetPermission**](#function-unsetpermission-22) ([**Permission**](classendstone_1_1Permission.md) & perm) <br>_Removes the specified permission from this attachment._  |




























## Public Functions Documentation




### function PermissionAttachment 

```C++
inline endstone::PermissionAttachment::PermissionAttachment (
    Plugin & plugin,
    Permissible & permissible
) 
```




<hr>



### function getPermissible 

_Gets the_ [_**Permissible**_](classendstone_1_1Permissible.md) _that this is attached to._
```C++
inline Permissible & endstone::PermissionAttachment::getPermissible () const
```





**Returns:**

[**Permissible**](classendstone_1_1Permissible.md) containing this attachment 





        

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

true if the permissible was removed successfully, false if it did not exist 





        

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
    Permission & perm,
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
    Permission & perm
) 
```



If the permission does not exist in this attachment, nothing will happen.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission_attachment.h`

