

# Class endstone::Permission



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Permission**](classendstone_1_1Permission.md)



_Represents a unique permission that may be attached to a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._

* `#include <endstone/permissions/permission.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const PermissionDefault | [**DefaultPermission**](#variable-defaultpermission)   = = PermissionDefault::Operator<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Permission**](#function-permission) (std::string name, std::string description="", PermissionDefault default\_value=DefaultPermission, std::unordered\_map&lt; std::string, bool &gt; children={}) <br> |
|  [**Permission**](classendstone_1_1Permission.md) \* | [**addParent**](#function-addparent-12) (std::string name, bool value) <br> |
|  void | [**addParent**](#function-addparent-22) ([**Permission**](classendstone_1_1Permission.md) & perm, bool value) const<br> |
|  std::unordered\_map&lt; std::string, bool &gt; & | [**getChildren**](#function-getchildren) () <br> |
|  PermissionDefault | [**getDefault**](#function-getdefault) () const<br> |
|  std::string | [**getDescription**](#function-getdescription) () const<br> |
|  std::string | [**getName**](#function-getname) () const<br> |
|  std::unordered\_set&lt; [**Permissible**](classendstone_1_1Permissible.md) \* &gt; | [**getPermissibles**](#function-getpermissibles) () const<br> |
|  void | [**init**](#function-init) ([**PluginManager**](classendstone_1_1PluginManager.md) & plugin\_manager) <br> |
|  void | [**recalculatePermissibles**](#function-recalculatepermissibles) () <br> |
|  void | [**setDefault**](#function-setdefault) (PermissionDefault value) <br> |
|  void | [**setDescription**](#function-setdescription) (std::string value) <br> |




























## Public Static Attributes Documentation




### variable DefaultPermission 

```C++
const PermissionDefault endstone::Permission::DefaultPermission;
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


```C++
inline Permission * endstone::Permission::addParent (
    std::string name,
    bool value
) 
```



Adds this permission to the specified parent permission.


If the parent permission does not exist, it will be created and registered.




**Parameters:**


* `name` Name of the parent permission 
* `value` The value to set this permission to 



**Returns:**

Parent permission it created or loaded 





        

<hr>



### function addParent [2/2]


```C++
inline void endstone::Permission::addParent (
    Permission & perm,
    bool value
) const
```



Adds this permission to the specified parent permission.




**Parameters:**


* `perm` Parent permission to register with 
* `value` The value to set this permission to 




        

<hr>



### function getChildren 


```C++
inline std::unordered_map< std::string, bool > & endstone::Permission::getChildren () 
```



Gets the children of this permission. If you change this map in any form, you must call [**recalculatePermissibles()**](classendstone_1_1Permission.md#function-recalculatepermissibles) to recalculate all Permissibles




**Returns:**

[**Permission**](classendstone_1_1Permission.md) children 





        

<hr>



### function getDefault 


```C++
inline PermissionDefault endstone::Permission::getDefault () const
```



Gets the default value of this permission.




**Returns:**

Default value of this permission. 





        

<hr>



### function getDescription 


```C++
inline std::string endstone::Permission::getDescription () const
```



Gets a brief description of this permission, may be empty




**Returns:**

Brief description of this permission 





        

<hr>



### function getName 


```C++
inline std::string endstone::Permission::getName () const
```



Returns the unique fully qualified name of this [**Permission**](classendstone_1_1Permission.md)




**Returns:**

Fully qualified name 





        

<hr>



### function getPermissibles 


```C++
inline std::unordered_set< Permissible * > endstone::Permission::getPermissibles () const
```



Gets a set containing every [**Permissible**](classendstone_1_1Permissible.md) that has this permission. This set cannot be modified.




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


```C++
inline void endstone::Permission::recalculatePermissibles () 
```



Recalculates all Permissibles that contain this permission.


This should be called after modifying the children, and is automatically called after modifying the default value 


        

<hr>



### function setDefault 


```C++
inline void endstone::Permission::setDefault (
    PermissionDefault value
) 
```



Sets the default value of this permission.


This will not be saved to disk, and is a temporary operation until the server reloads permissions. Changing this default will cause all Permissibles that contain this permission to recalculate their permissions




**Parameters:**


* `value` The new default to set 




        

<hr>



### function setDescription 


```C++
inline void endstone::Permission::setDescription (
    std::string value
) 
```



Sets the description of this permission.


This will not be saved to disk, and is a temporary operation until the server reloads permissions.




**Parameters:**


* `value` The new description to set 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/permissions/permission.h`

