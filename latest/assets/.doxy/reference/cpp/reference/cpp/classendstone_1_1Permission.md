

# Class endstone::Permission



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Permission**](classendstone_1_1Permission.md)



_Represents a unique permission that may be attached to a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._

* `#include <endstone/permissions/permission.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DefaultPermission**](#variable-defaultpermission)   = `PermissionDefault::Operator`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Permission**](#function-permission) (std::string name, std::string description="", [**PermissionDefault**](namespaceendstone.md#enum-permissiondefault) default\_value=DefaultPermission, std::unordered\_map&lt; std::string, [**bool**](classendstone_1_1Identifier.md) &gt; children={}) <br> |
|  [**Permission**](classendstone_1_1Permission.md) \* | [**addParent**](#function-addparent-12) (std::string name, [**bool**](classendstone_1_1Identifier.md) value) <br>_Adds this permission to the specified parent permission._  |
|  [**void**](classendstone_1_1Identifier.md) | [**addParent**](#function-addparent-22) ([**Permission**](classendstone_1_1Permission.md) & perm, [**bool**](classendstone_1_1Identifier.md) value) const<br>_Adds this permission to the specified parent permission._  |
|  std::unordered\_map&lt; std::string, [**bool**](classendstone_1_1Identifier.md) &gt; & | [**getChildren**](#function-getchildren) () <br>_Gets the children of this permission._  |
|  [**PermissionDefault**](namespaceendstone.md#enum-permissiondefault) | [**getDefault**](#function-getdefault) () const<br>_Gets the default value of this permission._  |
|  std::string | [**getDescription**](#function-getdescription) () const<br>_Gets a brief description of this permission, may be empty._  |
|  std::string | [**getName**](#function-getname) () const<br>_Returns the unique fully qualified name of this_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
|  std::unordered\_set&lt; [**Permissible**](classendstone_1_1Permissible.md) \* &gt; | [**getPermissibles**](#function-getpermissibles) () const<br>_Gets a set containing every_ [_**Permissible**_](classendstone_1_1Permissible.md) _that has this permission._ |
|  [**void**](classendstone_1_1Identifier.md) | [**init**](#function-init) ([**PluginManager**](classendstone_1_1PluginManager.md) & plugin\_manager) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**recalculatePermissibles**](#function-recalculatepermissibles) () <br>_Recalculates all Permissibles that contain this permission._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setDefault**](#function-setdefault) ([**PermissionDefault**](namespaceendstone.md#enum-permissiondefault) value) <br>_Sets the default value of this permission._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setDescription**](#function-setdescription) (std::string value) <br>_Sets the description of this permission._  |




























## Public Static Attributes Documentation




### variable DefaultPermission 

```C++
constexpr auto endstone::Permission::DefaultPermission;
```




<hr>
## Public Functions Documentation




### function Permission 

```C++
inline explicit endstone::Permission::Permission (
    std::string name,
    std::string description="",
    PermissionDefault default_value=DefaultPermission,
    std::unordered_map< std::string, bool > children={}
) 
```




<hr>



### function addParent [1/2]

_Adds this permission to the specified parent permission._ 
```C++
inline Permission * endstone::Permission::addParent (
    std::string name,
    bool value
) 
```



If the parent permission does not exist, it will be created and registered.




**Parameters:**


* `name` Name of the parent permission 
* `value` The value to set this permission to 



**Returns:**

Parent permission it created or loaded 





        

<hr>



### function addParent [2/2]

_Adds this permission to the specified parent permission._ 
```C++
inline void endstone::Permission::addParent (
    Permission & perm,
    bool value
) const
```





**Parameters:**


* `perm` Parent permission to register with 
* `value` The value to set this permission to 




        

<hr>



### function getChildren 

_Gets the children of this permission._ 
```C++
inline std::unordered_map< std::string, bool > & endstone::Permission::getChildren () 
```



If you change this map in any form, you must call [**recalculatePermissibles()**](classendstone_1_1Permission.md#function-recalculatepermissibles) to recalculate all Permissibles.




**Returns:**

[**Permission**](classendstone_1_1Permission.md) children 





        

<hr>



### function getDefault 

_Gets the default value of this permission._ 
```C++
inline PermissionDefault endstone::Permission::getDefault () const
```





**Returns:**

Default value of this permission. 





        

<hr>



### function getDescription 

_Gets a brief description of this permission, may be empty._ 
```C++
inline std::string endstone::Permission::getDescription () const
```





**Returns:**

Brief description of this permission 





        

<hr>



### function getName 

_Returns the unique fully qualified name of this_ [_**Permission**_](classendstone_1_1Permission.md) _._
```C++
inline std::string endstone::Permission::getName () const
```





**Returns:**

Fully qualified name 





        

<hr>



### function getPermissibles 

_Gets a set containing every_ [_**Permissible**_](classendstone_1_1Permissible.md) _that has this permission._
```C++
inline std::unordered_set< Permissible * > endstone::Permission::getPermissibles () const
```



This set cannot be modified.




**Returns:**

Set containing permissibles with this permission 





        

<hr>



### function init 

```C++
inline void endstone::Permission::init (
    PluginManager & plugin_manager
) 
```




<hr>



### function recalculatePermissibles 

_Recalculates all Permissibles that contain this permission._ 
```C++
inline void endstone::Permission::recalculatePermissibles () 
```



This should be called after modifying the children, and is automatically called after modifying the default value 


        

<hr>



### function setDefault 

_Sets the default value of this permission._ 
```C++
inline void endstone::Permission::setDefault (
    PermissionDefault value
) 
```



This will not be saved to disk, and is a temporary operation until the server reloads permissions. Changing this default will cause all Permissibles that contain this permission to recalculate their permissions




**Parameters:**


* `value` The new default to set 




        

<hr>



### function setDescription 

_Sets the description of this permission._ 
```C++
inline void endstone::Permission::setDescription (
    std::string value
) 
```



This will not be saved to disk, and is a temporary operation until the server reloads permissions.




**Parameters:**


* `value` The new description to set 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission.h`

