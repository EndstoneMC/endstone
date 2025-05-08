

# Class endstone::Enchantment



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Enchantment**](classendstone_1_1Enchantment.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Vector.md) | [**canEnchantItem**](#function-canenchantitem) ([**const**](classendstone_1_1Vector.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**conflictsWith**](#function-conflictswith) ([**const**](classendstone_1_1Vector.md) [**Enchantment**](classendstone_1_1Enchantment.md) & other) const = 0<br>_Check if this enchantment conflicts with another enchantment._  |
| virtual [**NamespacedKey**](classendstone_1_1NamespacedKey.md) | [**getKey**](#function-getkey) () const = 0<br>_Return the namespaced identifier for this enchantment._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxLevel**](#function-getmaxlevel) () const = 0<br>_Gets the maximum level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may become._ |
| virtual [**int**](classendstone_1_1Vector.md) | [**getStartLevel**](#function-getstartlevel) () const = 0<br>_Gets the level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _should start at (also known as minimum level)._ |
| virtual  | [**~Enchantment**](#function-enchantment) () = default<br> |




























## Public Functions Documentation




### function canEnchantItem 

```C++
virtual bool endstone::Enchantment::canEnchantItem (
    const  ItemStack & item
) const = 0
```



Checks if this [**Enchantment**](classendstone_1_1Enchantment.md) may be applied to the given [**ItemStack**](classendstone_1_1ItemStack.md). 


This does not check if it conflicts with any enchantments already applied to the item.




**Parameters:**


* `item` Item to test 



**Returns:**

True if the enchantment may be applied, otherwise False 





        

<hr>



### function conflictsWith 

_Check if this enchantment conflicts with another enchantment._ 
```C++
virtual bool endstone::Enchantment::conflictsWith (
    const  Enchantment & other
) const = 0
```





**Parameters:**


* `other` The enchantment to check against 



**Returns:**

True if there is a conflict. 





        

<hr>



### function getKey 

_Return the namespaced identifier for this enchantment._ 
```C++
virtual NamespacedKey endstone::Enchantment::getKey () const = 0
```





**Returns:**

this enchantment's key 





        

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



### function ~Enchantment 

```C++
virtual endstone::Enchantment::~Enchantment () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/enchantments/enchantment.h`

