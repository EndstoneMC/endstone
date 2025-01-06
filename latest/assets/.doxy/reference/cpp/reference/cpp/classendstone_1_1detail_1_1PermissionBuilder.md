

# Class endstone::detail::PermissionBuilder



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**detail**](namespaceendstone_1_1detail.md) **>** [**PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionBuilder**](#function-permissionbuilder) (std::string name) <br> |
|  [**Permission**](classendstone_1_1Permission.md) | [**build**](#function-build) () const<br> |
|  [**PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md) & | [**children**](#function-children) (const std::string & name, bool value) <br> |
|  [**PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md) & | [**default\_**](#function-default_) (PermissionDefault default\_value) <br> |
|  [**PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md) & | [**description**](#function-description) (std::string description) <br> |




























## Public Functions Documentation




### function PermissionBuilder 

```C++
inline explicit endstone::detail::PermissionBuilder::PermissionBuilder (
    std::string name
) 
```




<hr>



### function build 

```C++
inline Permission endstone::detail::PermissionBuilder::build () const
```




<hr>



### function children 

```C++
inline PermissionBuilder & endstone::detail::PermissionBuilder::children (
    const std::string & name,
    bool value
) 
```




<hr>



### function default\_ 

```C++
inline PermissionBuilder & endstone::detail::PermissionBuilder::default_ (
    PermissionDefault default_value
) 
```




<hr>



### function description 

```C++
inline PermissionBuilder & endstone::detail::PermissionBuilder::description (
    std::string description
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

