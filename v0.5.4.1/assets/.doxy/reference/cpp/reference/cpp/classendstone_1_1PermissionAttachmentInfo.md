

# Class endstone::PermissionAttachmentInfo



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md)



_Holds information on a permission and which_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _provides it._

* `#include <endstone/permissions/permission_attachment_info.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionAttachmentInfo**](#function-permissionattachmentinfo) ([**Permissible**](classendstone_1_1Permissible.md) & permissible, std::string permission, [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* attachment, bool value) <br> |
|  [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**getAttachment**](#function-getattachment) () const<br> |
|  [**Permissible**](classendstone_1_1Permissible.md) & | [**getPermissible**](#function-getpermissible) () const<br> |
|  std::string | [**getPermission**](#function-getpermission) () const<br> |
|  bool | [**getValue**](#function-getvalue) () const<br> |




























## Public Functions Documentation




### function PermissionAttachmentInfo 

```C++
inline endstone::PermissionAttachmentInfo::PermissionAttachmentInfo (
    Permissible & permissible,
    std::string permission,
    PermissionAttachment * attachment,
    bool value
) 
```




<hr>



### function getAttachment 


```C++
inline PermissionAttachment * endstone::PermissionAttachmentInfo::getAttachment () const
```



Gets the attachment providing this permission. This may be null for default permissions (usually parent permissions).




**Returns:**

Attachment 





        

<hr>



### function getPermissible 


```C++
inline Permissible & endstone::PermissionAttachmentInfo::getPermissible () const
```



Gets the permissible this is attached to




**Returns:**

[**Permissible**](classendstone_1_1Permissible.md) this permission is for 





        

<hr>



### function getPermission 


```C++
inline std::string endstone::PermissionAttachmentInfo::getPermission () const
```



Gets the permission being set




**Returns:**

Name of the permission 





        

<hr>



### function getValue 


```C++
inline bool endstone::PermissionAttachmentInfo::getValue () const
```



Gets the value of this permission




**Returns:**

Value of the permission 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission_attachment_info.h`

