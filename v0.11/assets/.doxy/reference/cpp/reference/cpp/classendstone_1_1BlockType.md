

# Class endstone::BlockType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockType**](classendstone_1_1BlockType.md)








Inherits the following classes: [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)
















## Public Types inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| typedef [**Identifier**](classendstone_1_1Identifier.md)&lt; T &gt; | [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id)  <br> |










## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  auto | [**Air**](#variable-air)   = `BlockTypeId::minecraft("air")`<br> |




























## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; | [**createBlockData**](#function-createblockdata) () const = 0<br>_Creates a new_ [_**BlockData**_](classendstone_1_1BlockData.md) _instance for this block type, with all properties initialized to unspecified defaults._ |
| virtual bool | [**hasItemType**](#function-hasitemtype) () const = 0<br>_Returns true if this_ [_**BlockType**_](classendstone_1_1BlockType.md) _has a corresponding_[_**ItemType**_](classendstone_1_1ItemType.md) _._ |


## Public Functions inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| virtual Id | [**getId**](classendstone_1_1Registry_1_1Type.md#function-getid) () const = 0<br>_Return the identifier of this object._  |
| virtual std::string | [**getTranslationKey**](classendstone_1_1Registry_1_1Type.md#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|   | [**operator Id**](classendstone_1_1Registry_1_1Type.md#function-operator-id) () const<br> |
|  bool | [**operator!=**](classendstone_1_1Registry_1_1Type.md#function-operator) (const Id & other) const<br> |
|  bool | [**operator!=**](classendstone_1_1Registry_1_1Type.md#function-operator_1) (const T & other) const<br> |
|  bool | [**operator==**](classendstone_1_1Registry_1_1Type.md#function-operator_2) (const Id & other) const<br> |
|  bool | [**operator==**](classendstone_1_1Registry_1_1Type.md#function-operator_3) (const T & other) const<br> |
| virtual  | [**~Type**](classendstone_1_1Registry_1_1Type.md#function-type) () = default<br> |




## Public Static Functions inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
|  const T \* | [**get**](classendstone_1_1Registry_1_1Type.md#function-get) (Id id) <br>_Look up a registry entry by identifier._  |


















































## Public Static Attributes Documentation




### variable Air 

```C++
auto endstone::BlockType::Air;
```




<hr>
## Public Functions Documentation




### function createBlockData 

_Creates a new_ [_**BlockData**_](classendstone_1_1BlockData.md) _instance for this block type, with all properties initialized to unspecified defaults._
```C++
virtual std::unique_ptr< BlockData > endstone::BlockType::createBlockData () const = 0
```





**Returns:**

new data instance 





        

<hr>



### function hasItemType 

_Returns true if this_ [_**BlockType**_](classendstone_1_1BlockType.md) _has a corresponding_[_**ItemType**_](classendstone_1_1ItemType.md) _._
```C++
virtual bool endstone::BlockType::hasItemType () const = 0
```





**Returns:**

true if there is a corresponding [**ItemType**](classendstone_1_1ItemType.md), otherwise false 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block_type.h`

