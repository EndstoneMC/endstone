

# Class endstone::detail::PermissionBuilder



[**ClassList**](annotated.md) **>** [**PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PermissionBuilder**](#function-permissionbuilder) (std::string name) <br> |
|  [**Permission**](classendstone_1_1Permission.md) | [**build**](#function-build) () const<br> |
|  PermissionBuilder & | [**children**](#function-children) ([**const**](classendstone_1_1Vector.md) std::string & name, [**bool**](classendstone_1_1Vector.md) value) <br> |
|  PermissionBuilder & | [**default\_**](#function-default_) (PermissionDefault default\_value) <br> |
|  PermissionBuilder & | [**description**](#function-description) (std::string description) <br> |




























## Public Functions Documentation




### function PermissionBuilder 

```C++
inline explicit PermissionBuilder::PermissionBuilder (
    std::string name
) 
```




<hr>



### function build 

```C++
inline Permission PermissionBuilder::build () const
```




<hr>



### function children 

```C++
inline PermissionBuilder & PermissionBuilder::children (
    const std::string & name,
    bool value
) 
```




<hr>



### function default\_ 

```C++
inline PermissionBuilder & PermissionBuilder::default_ (
    PermissionDefault default_value
) 
```




<hr>



### function description 

```C++
inline PermissionBuilder & PermissionBuilder::description (
    std::string description
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

