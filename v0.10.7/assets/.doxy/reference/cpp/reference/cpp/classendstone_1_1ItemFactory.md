

# Class endstone::ItemFactory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemFactory**](classendstone_1_1ItemFactory.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**asMetaFor**](#function-asmetafor) (const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta, const [**ItemType**](classendstone_1_1ItemType.md) & type) const = 0<br>_Returns an appropriate item meta for the specified item type._  |
| virtual bool | [**equals**](#function-equals) (const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta1, const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta2) const = 0<br>_This method is used to compare two_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _objects._ |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) (const [**ItemType**](classendstone_1_1ItemType.md) & type) const = 0<br>_This creates a new item meta for the item type._  |
| virtual bool | [**isApplicable**](#function-isapplicable) (const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta, const [**ItemType**](classendstone_1_1ItemType.md) & type) const = 0<br>_This method checks the item meta to confirm that it is applicable (no data lost if applied) to the specified_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual  | [**~ItemFactory**](#function-itemfactory) () = default<br> |




























## Public Functions Documentation




### function asMetaFor 

_Returns an appropriate item meta for the specified item type._ 
```C++
virtual std::unique_ptr< ItemMeta > endstone::ItemFactory::asMetaFor (
    const ItemMeta * meta,
    const ItemType & type
) const = 0
```



The item meta returned will always be a valid meta for a given [**ItemStack**](classendstone_1_1ItemStack.md) of the specified item type. It may be a more or less specific meta, and could also be the same meta or meta type as the parameter. The item meta returned will also always be the most appropriate meta.




**Parameters:**


* `meta` the meta to convert 
* `type` the item type to convert the meta for 



**Returns:**

An appropriate item meta for the specified item type. 





        

<hr>



### function equals 

_This method is used to compare two_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _objects._
```C++
virtual bool endstone::ItemFactory::equals (
    const ItemMeta * meta1,
    const ItemMeta * meta2
) const = 0
```





**Parameters:**


* `meta1` First meta to compare, and may be null to indicate no data 
* `meta2` Second meta to compare, and may be null to indicate no data 



**Returns:**

false if one of the meta has data the other does not, otherwise true 





        

<hr>



### function getItemMeta 

_This creates a new item meta for the item type._ 
```C++
virtual std::unique_ptr< ItemMeta > endstone::ItemFactory::getItemMeta (
    const ItemType & type
) const = 0
```





**Parameters:**


* `type` The item type to consider as base for the meta 



**Returns:**

a new [**ItemMeta**](classendstone_1_1ItemMeta.md) that could be applied to an item stack of the specified item type 





        

<hr>



### function isApplicable 

_This method checks the item meta to confirm that it is applicable (no data lost if applied) to the specified_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
virtual bool endstone::ItemFactory::isApplicable (
    const ItemMeta * meta,
    const ItemType & type
) const = 0
```





**Parameters:**


* `meta` Meta to check 
* `type` The item type that meta will be applied to 



**Returns:**

true if the meta can be applied without losing data, false otherwise 





        

<hr>



### function ~ItemFactory 

```C++
virtual endstone::ItemFactory::~ItemFactory () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_factory.h`

