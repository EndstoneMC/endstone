

# Class endstone::ItemFactory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemFactory**](classendstone_1_1ItemFactory.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**asMetaFor**](#function-asmetafor) ([**const**](classendstone_1_1Vector.md) std::string\_view type, [**const**](classendstone_1_1Vector.md) [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
|  std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) ([**const**](classendstone_1_1Vector.md) std::string\_view type, [**const**](classendstone_1_1Vector.md) [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta=[**nullptr**](classendstone_1_1Vector.md)) <br> |


























## Public Static Functions Documentation




### function asMetaFor 

```C++
static inline std::unique_ptr< ItemMeta > endstone::ItemFactory::asMetaFor (
    const std::string_view type,
    const  ItemMeta * meta
) 
```




<hr>



### function getItemMeta 

```C++
static inline std::unique_ptr< ItemMeta > endstone::ItemFactory::getItemMeta (
    const std::string_view type,
    const  ItemMeta * meta=nullptr
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_factory.h`

