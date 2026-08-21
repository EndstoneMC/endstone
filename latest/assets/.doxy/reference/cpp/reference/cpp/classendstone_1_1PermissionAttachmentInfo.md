

# Class endstone::PermissionAttachmentInfo



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md)



_Holds information on a permission and which_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _provides it._

* `#include <endstone/permissions/permission_attachment_info.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionAttachmentInfo**](#function-permissionattachmentinfo) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Permissible**](classendstone_1_1Permissible.md) &gt; & permissible, std::string permission, [**Nullable**](classendstone_1_1Nullable.md)&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) &gt; attachment, [**bool**](classendstone_1_1Identifier.md) value) <br> |
|  [**Nullable**](classendstone_1_1Nullable.md)&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) &gt; | [**getAttachment**](#function-getattachment) () const<br>_Gets the attachment providing this permission._  |
|  [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Permissible**](classendstone_1_1Permissible.md) &gt; | [**getPermissible**](#function-getpermissible) () const<br>_Gets the permissible this is attached to._  |
|  std::string | [**getPermission**](#function-getpermission) () const<br>_Gets the permission being set._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**getValue**](#function-getvalue) () const<br>_Gets the value of this permission._  |




























## Public Functions Documentation




### function PermissionAttachmentInfo 

```C++
inline endstone::PermissionAttachmentInfo::PermissionAttachmentInfo (
    const  NotNull < Permissible > & permissible,
    std::string permission,
    Nullable < PermissionAttachment > attachment,
    bool value
) 
```




<hr>



### function getAttachment 

_Gets the attachment providing this permission._ 
```C++
inline Nullable < PermissionAttachment > endstone::PermissionAttachmentInfo::getAttachment () const
```



This may be nullptr for default permissions (usually parent permissions).




**Returns:**

Attachment 





        

<hr>



### function getPermissible 

_Gets the permissible this is attached to._ 
```C++
inline Nullable < Permissible > endstone::PermissionAttachmentInfo::getPermissible () const
```





**Returns:**

[**Permissible**](classendstone_1_1Permissible.md) this permission is for, or nullptr if it no longer exists 





        

<hr>



### function getPermission 

_Gets the permission being set._ 
```C++
inline std::string endstone::PermissionAttachmentInfo::getPermission () const
```





**Returns:**

Name of the permission 





        

<hr>



### function getValue 

_Gets the value of this permission._ 
```C++
inline bool endstone::PermissionAttachmentInfo::getValue () const
```





**Returns:**

Value of the permission 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission_attachment_info.h`

