

# Class endstone::ItemStack::Impl



[**ClassList**](annotated.md) **>** [**Impl**](classendstone_1_1ItemStack_1_1Impl.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; Impl &gt; | [**clone**](#function-clone) () const = 0<br> |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const = 0<br> |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getData**](#function-getdata) () const = 0<br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) () const = 0<br> |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br> |
| virtual [**CompoundTag**](classendstone_1_1CompoundTag.md) | [**getNbt**](#function-getnbt) () const = 0<br> |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br> |
| virtual [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & | [**getType**](#function-gettype) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasItemMeta**](#function-hasitemmeta) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isSimilar**](#function-issimilar) ([**const**](classendstone_1_1Identifier.md) Impl & other) const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setAmount**](#function-setamount) ([**int**](classendstone_1_1Identifier.md) amount) = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setData**](#function-setdata) ([**int**](classendstone_1_1Identifier.md) data) = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**setItemMeta**](#function-setitemmeta) ([**const**](classendstone_1_1Identifier.md) [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNbt**](#function-setnbt) ([**const**](classendstone_1_1Identifier.md) [**CompoundTag**](classendstone_1_1CompoundTag.md) & nbt) = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setType**](#function-settype) ([**ItemTypeId**](namespaceendstone.md#typedef-itemtypeid) type) = 0<br> |
| virtual  | [**~Impl**](#function-impl) () = default<br> |




























## Public Functions Documentation




### function clone 

```C++
virtual std::unique_ptr< Impl > Impl::clone () const = 0
```




<hr>



### function getAmount 

```C++
virtual int Impl::getAmount () const = 0
```




<hr>



### function getData 

```C++
virtual int Impl::getData () const = 0
```




<hr>



### function getItemMeta 

```C++
virtual std::unique_ptr< ItemMeta > Impl::getItemMeta () const = 0
```




<hr>



### function getMaxStackSize 

```C++
virtual int Impl::getMaxStackSize () const = 0
```




<hr>



### function getNbt 

```C++
virtual CompoundTag Impl::getNbt () const = 0
```




<hr>



### function getTranslationKey 

```C++
virtual std::string Impl::getTranslationKey () const = 0
```




<hr>



### function getType 

```C++
virtual const  ItemType & Impl::getType () const = 0
```




<hr>



### function hasItemMeta 

```C++
virtual bool Impl::hasItemMeta () const = 0
```




<hr>



### function isSimilar 

```C++
virtual bool Impl::isSimilar (
    const Impl & other
) const = 0
```




<hr>



### function setAmount 

```C++
virtual void Impl::setAmount (
    int amount
) = 0
```




<hr>



### function setData 

```C++
virtual void Impl::setData (
    int data
) = 0
```




<hr>



### function setItemMeta 

```C++
virtual bool Impl::setItemMeta (
    const  ItemMeta * meta
) = 0
```




<hr>



### function setNbt 

```C++
virtual void Impl::setNbt (
    const  CompoundTag & nbt
) = 0
```




<hr>



### function setType 

```C++
virtual void Impl::setType (
    ItemTypeId type
) = 0
```




<hr>



### function ~Impl 

```C++
virtual Impl::~Impl () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_stack.h`

