

# Class endstone::ItemType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemType**](classendstone_1_1ItemType.md)




























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Air**](#variable-air)   = `ItemTypeId::minecraft("air")`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**ItemStack**](classendstone_1_1ItemStack.md) | [**createItemStack**](#function-createitemstack-12) () const = 0<br>_Constructs a new_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with this item type that has the amount 1._ |
| virtual [**ItemStack**](classendstone_1_1ItemStack.md) | [**createItemStack**](#function-createitemstack-22) ([**int**](classendstone_1_1Identifier.md) amount) const = 0<br>_Constructs a new_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with this item type._ |
| virtual [**ItemTypeId**](namespaceendstone.md#typedef-itemtypeid) | [**getId**](#function-getid) () const = 0<br>_Return the identifier of this item type._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMaxDurability**](#function-getmaxdurability) () const = 0<br>_Gets the maximum durability of this item type._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br>_Gets the maximum amount of this item type that can be held in a stack._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey-12) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey-22) ([**int**](classendstone_1_1Identifier.md) data) const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|   | [**operator ItemTypeId**](#function-operator-itemtypeid) () const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**ItemTypeId**](namespaceendstone.md#typedef-itemtypeid) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_2) ([**const**](classendstone_1_1Identifier.md) [**ItemTypeId**](namespaceendstone.md#typedef-itemtypeid) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & other) const<br> |
| virtual  | [**~ItemType**](#function-itemtype) () = default<br> |




























## Public Static Attributes Documentation




### variable Air 

```C++
constexpr auto endstone::ItemType::Air;
```




<hr>
## Public Functions Documentation




### function createItemStack [1/2]

_Constructs a new_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with this item type that has the amount 1._
```C++
virtual ItemStack endstone::ItemType::createItemStack () const = 0
```





**Returns:**

the constructed item stack. 





        

<hr>



### function createItemStack [2/2]

_Constructs a new_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with this item type._
```C++
virtual ItemStack endstone::ItemType::createItemStack (
    int amount
) const = 0
```





**Parameters:**


* `amount` the amount in the stack 



**Returns:**

An [**ItemStack**](classendstone_1_1ItemStack.md) of this item type 





        

<hr>



### function getId 

_Return the identifier of this item type._ 
```C++
virtual ItemTypeId endstone::ItemType::getId () const = 0
```





**Returns:**

this item's identifier 





        

<hr>



### function getMaxDurability 

_Gets the maximum durability of this item type._ 
```C++
virtual int endstone::ItemType::getMaxDurability () const = 0
```





**Returns:**

Maximum durability for this item type 





        

<hr>



### function getMaxStackSize 

_Gets the maximum amount of this item type that can be held in a stack._ 
```C++
virtual int endstone::ItemType::getMaxStackSize () const = 0
```





**Returns:**

Maximum stack size for this item type 





        

<hr>



### function getTranslationKey [1/2]

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::ItemType::getTranslationKey () const = 0
```





**Returns:**

the translation key 





        

<hr>



### function getTranslationKey [2/2]

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::ItemType::getTranslationKey (
    int data
) const = 0
```





**Parameters:**


* `data` Data for this item type



**Returns:**

the translation key 





        

<hr>



### function operator ItemTypeId 

```C++
inline endstone::ItemType::operator ItemTypeId () const
```




<hr>



### function operator!= 

```C++
inline bool endstone::ItemType::operator!= (
    const  ItemTypeId & other
) const
```




<hr>



### function operator!= 

```C++
inline bool endstone::ItemType::operator!= (
    const  ItemType & other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::ItemType::operator== (
    const  ItemTypeId & other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::ItemType::operator== (
    const  ItemType & other
) const
```




<hr>



### function ~ItemType 

```C++
virtual endstone::ItemType::~ItemType () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_type.h`

