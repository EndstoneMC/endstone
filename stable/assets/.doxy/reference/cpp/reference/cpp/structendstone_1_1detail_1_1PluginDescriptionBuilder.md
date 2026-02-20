

# Struct endstone::detail::PluginDescriptionBuilder



[**ClassList**](annotated.md) **>** [**PluginDescriptionBuilder**](structendstone_1_1detail_1_1PluginDescriptionBuilder.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::vector&lt; std::string &gt; | [**authors**](#variable-authors)  <br> |
|  std::unordered\_map&lt; std::string, CommandBuilder &gt; | [**commands**](#variable-commands)  <br> |
|  std::vector&lt; std::string &gt; | [**contributors**](#variable-contributors)  <br> |
|  [**PermissionDefault**](namespaceendstone.md#enum-permissiondefault) | [**default\_permission**](#variable-default_permission)   = `PermissionDefault::Operator`<br> |
|  std::vector&lt; std::string &gt; | [**depend**](#variable-depend)  <br> |
|  std::string | [**description**](#variable-description)  <br> |
|  [**PluginLoadOrder**](namespaceendstone.md#enum-pluginloadorder) | [**load**](#variable-load)   = `PluginLoadOrder::PostWorld`<br> |
|  std::vector&lt; std::string &gt; | [**load\_before**](#variable-load_before)  <br> |
|  std::unordered\_map&lt; std::string, PermissionBuilder &gt; | [**permissions**](#variable-permissions)  <br> |
|  std::string | [**prefix**](#variable-prefix)  <br> |
|  std::vector&lt; std::string &gt; | [**provides**](#variable-provides)  <br> |
|  std::vector&lt; std::string &gt; | [**soft\_depend**](#variable-soft_depend)  <br> |
|  std::string | [**website**](#variable-website)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**PluginDescription**](classendstone_1_1PluginDescription.md) | [**build**](#function-build) (std::string name, std::string version) const<br> |
|  CommandBuilder & | [**command**](#function-command) (std::string name) <br> |
|  PermissionBuilder & | [**permission**](#function-permission) (std::string name) <br> |




























## Public Attributes Documentation




### variable authors 

```C++
std::vector<std::string> endstone::detail::PluginDescriptionBuilder::authors;
```




<hr>



### variable commands 

```C++
std::unordered_map<std::string, CommandBuilder> endstone::detail::PluginDescriptionBuilder::commands;
```




<hr>



### variable contributors 

```C++
std::vector<std::string> endstone::detail::PluginDescriptionBuilder::contributors;
```




<hr>



### variable default\_permission 

```C++
PermissionDefault endstone::detail::PluginDescriptionBuilder::default_permission;
```




<hr>



### variable depend 

```C++
std::vector<std::string> endstone::detail::PluginDescriptionBuilder::depend;
```




<hr>



### variable description 

```C++
std::string endstone::detail::PluginDescriptionBuilder::description;
```




<hr>



### variable load 

```C++
PluginLoadOrder endstone::detail::PluginDescriptionBuilder::load;
```




<hr>



### variable load\_before 

```C++
std::vector<std::string> endstone::detail::PluginDescriptionBuilder::load_before;
```




<hr>



### variable permissions 

```C++
std::unordered_map<std::string, PermissionBuilder> endstone::detail::PluginDescriptionBuilder::permissions;
```




<hr>



### variable prefix 

```C++
std::string endstone::detail::PluginDescriptionBuilder::prefix;
```




<hr>



### variable provides 

```C++
std::vector<std::string> endstone::detail::PluginDescriptionBuilder::provides;
```




<hr>



### variable soft\_depend 

```C++
std::vector<std::string> endstone::detail::PluginDescriptionBuilder::soft_depend;
```




<hr>



### variable website 

```C++
std::string endstone::detail::PluginDescriptionBuilder::website;
```




<hr>
## Public Functions Documentation




### function build 

```C++
inline PluginDescription PluginDescriptionBuilder::build (
    std::string name,
    std::string version
) const
```




<hr>



### function command 

```C++
inline CommandBuilder & PluginDescriptionBuilder::command (
    std::string name
) 
```




<hr>



### function permission 

```C++
inline PermissionBuilder & PluginDescriptionBuilder::permission (
    std::string name
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

