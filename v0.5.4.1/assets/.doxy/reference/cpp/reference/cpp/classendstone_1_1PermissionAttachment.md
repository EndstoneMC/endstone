

# Class endstone::PermissionAttachment



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md)



_Holds information about a permission attachment on a_ [_**Permissible**_](classendstone_1_1Permissible.md) _object._

* `#include <endstone/permissions/permission_attachment.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionAttachment**](#function-permissionattachment) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**Permissible**](classendstone_1_1Permissible.md) & permissible) <br> |
|  [**Permissible**](classendstone_1_1Permissible.md) & | [**getPermissible**](#function-getpermissible) () const<br> |
|  std::unordered\_map&lt; std::string, bool &gt; | [**getPermissions**](#function-getpermissions) () const<br> |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br> |
|  PermissionRemovedExecutor | [**getRemovalCallback**](#function-getremovalcallback) () const<br> |
|  bool | [**remove**](#function-remove) () <br> |
|  void | [**setPermission**](#function-setpermission-12) (std::string name, bool value) <br> |
|  void | [**setPermission**](#function-setpermission-22) ([**Permission**](classendstone_1_1Permission.md) & perm, bool value) <br> |
|  void | [**setRemovalCallback**](#function-setremovalcallback) (PermissionRemovedExecutor ex) <br> |
|  void | [**unsetPermission**](#function-unsetpermission-12) (std::string name) <br> |
|  void | [**unsetPermission**](#function-unsetpermission-22) ([**Permission**](classendstone_1_1Permission.md) & perm) <br> |




























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


```C++
inline Permissible & endstone::PermissionAttachment::getPermissible () const
```



Gets the [**Permissible**](classendstone_1_1Permissible.md) that this is attached to




**Returns:**

[**Permissible**](classendstone_1_1Permissible.md) containing this attachment 





        

<hr>



### function getPermissions 


```C++
inline std::unordered_map< std::string, bool > endstone::PermissionAttachment::getPermissions () const
```



Gets a copy of all set permissions and values contained within this attachment. This map may be modified but will not affect the attachment, as it is a copy.




**Returns:**

Copy of all permissions and values expressed by this attachment 





        

<hr>



### function getPlugin 


```C++
inline Plugin & endstone::PermissionAttachment::getPlugin () const
```



Gets the plugin responsible for this attachment




**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) responsible for this permission attachment 





        

<hr>



### function getRemovalCallback 


```C++
inline PermissionRemovedExecutor endstone::PermissionAttachment::getRemovalCallback () const
```



Gets the class that was previously set to be called when this attachment was removed from a [**Permissible**](classendstone_1_1Permissible.md). May be empty.




**Returns:**

Executor to be called when this is removed 





        

<hr>



### function remove 


```C++
inline bool endstone::PermissionAttachment::remove () 
```



Removes this attachment from its registered [**Permissible**](classendstone_1_1Permissible.md)




**Returns:**

true if the permissible was removed successfully, false if it did not exist 





        

<hr>



### function setPermission [1/2]


```C++
inline void endstone::PermissionAttachment::setPermission (
    std::string name,
    bool value
) 
```



Sets a permission to the given value, by its fully qualified name




**Parameters:**


* `name` Name of the permission 
* `value` New value of the permission 




        

<hr>



### function setPermission [2/2]


```C++
inline void endstone::PermissionAttachment::setPermission (
    Permission & perm,
    bool value
) 
```



Sets a permission to the given value




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to set 
* `value` New value of the permission 




        

<hr>



### function setRemovalCallback 


```C++
inline void endstone::PermissionAttachment::setRemovalCallback (
    PermissionRemovedExecutor ex
) 
```



Sets an executor to be called for when this attachment is removed from a [**Permissible**](classendstone_1_1Permissible.md). May be empty.




**Parameters:**


* `ex` Executor to be called when this is removed 




        

<hr>



### function unsetPermission [1/2]


```C++
inline void endstone::PermissionAttachment::unsetPermission (
    std::string name
) 
```



Removes the specified permission from this attachment. If the permission does not exist in this attachment, nothing will happen.




**Parameters:**


* `name` Name of the permission to remove 




        

<hr>



### function unsetPermission [2/2]


```C++
inline void endstone::PermissionAttachment::unsetPermission (
    Permission & perm
) 
```



Removes the specified permission from this attachment. If the permission does not exist in this attachment, nothing will happen.




**Parameters:**


* `perm` [**Permission**](classendstone_1_1Permission.md) to remove 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission_attachment.h`

