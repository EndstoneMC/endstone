

# Class endstone::Enchantment



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Enchantment**](classendstone_1_1Enchantment.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**canEnchantItem**](#function-canenchantitem) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Checks if this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may be applied to the given_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual bool | [**conflictsWith**](#function-conflictswith) (const [**Enchantment**](classendstone_1_1Enchantment.md) & other) const = 0<br>_Check if this enchantment conflicts with another enchantment._  |
| virtual std::string | [**getId**](#function-getid) () const = 0<br>_Return the identifier for this enchantment._  |
| virtual int | [**getMaxLevel**](#function-getmaxlevel) () const = 0<br>_Gets the maximum level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may become._ |
| virtual int | [**getStartLevel**](#function-getstartlevel) () const = 0<br>_Gets the level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _should start at (also known as minimum level)._ |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
| virtual  | [**~Enchantment**](#function-enchantment) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  const [**Enchantment**](classendstone_1_1Enchantment.md) \* | [**get**](#function-get) (const std::string & name) <br>_Attempts to get the_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _with the given name._ |


























## Public Functions Documentation




### function canEnchantItem 

_Checks if this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may be applied to the given_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
virtual bool endstone::Enchantment::canEnchantItem (
    const ItemStack & item
) const = 0
```



This does not check if it conflicts with any enchantments already applied to the item.




**Parameters:**


* `item` [**Item**](classendstone_1_1Item.md) to test 



**Returns:**

True if the enchantment may be applied, otherwise False 





        

<hr>



### function conflictsWith 

_Check if this enchantment conflicts with another enchantment._ 
```C++
virtual bool endstone::Enchantment::conflictsWith (
    const Enchantment & other
) const = 0
```





**Parameters:**


* `other` The enchantment to check against 



**Returns:**

True if there is a conflict. 





        

<hr>



### function getId 

_Return the identifier for this enchantment._ 
```C++
virtual std::string endstone::Enchantment::getId () const = 0
```





**Returns:**

this enchantment's id 





        

<hr>



### function getMaxLevel 

_Gets the maximum level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may become._
```C++
virtual int endstone::Enchantment::getMaxLevel () const = 0
```





**Returns:**

Maximum level of the [**Enchantment**](classendstone_1_1Enchantment.md) 





        

<hr>



### function getStartLevel 

_Gets the level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _should start at (also known as minimum level)._
```C++
virtual int endstone::Enchantment::getStartLevel () const = 0
```





**Returns:**

Starting level of the [**Enchantment**](classendstone_1_1Enchantment.md) 





        

<hr>



### function getTranslationKey 

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::Enchantment::getTranslationKey () const = 0
```





**Returns:**

the translation key 





        

<hr>



### function ~Enchantment 

```C++
virtual endstone::Enchantment::~Enchantment () = default
```




<hr>
## Public Static Functions Documentation




### function get 

_Attempts to get the_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _with the given name._
```C++
static inline const Enchantment * endstone::Enchantment::get (
    const std::string & name
) 
```





**Note:**

This is a normal lookup, names must be the precise name.




**Parameters:**


* `name` Name of the [**Enchantment**](classendstone_1_1Enchantment.md) to get



**Returns:**

[**ItemType**](classendstone_1_1ItemType.md) if found, or nullptr 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/enchantments/enchantment.h`

