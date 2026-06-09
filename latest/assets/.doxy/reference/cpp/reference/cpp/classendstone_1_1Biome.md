

# Class endstone::Biome



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Biome**](classendstone_1_1Biome.md)








Inherits the following classes: [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)
















## Public Types inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| typedef [**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id)  <br>_The identifier type used to look up this object in its registry._  |








































## Public Functions inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| virtual [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) | [**getId**](classendstone_1_1Registry_1_1Type.md#function-getid) () const = 0<br>_Return the identifier of this object._  |
| virtual std::string | [**getTranslationKey**](classendstone_1_1Registry_1_1Type.md#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|   | [**operator Id**](classendstone_1_1Registry_1_1Type.md#function-operator-id) () const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](classendstone_1_1Registry_1_1Type.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](classendstone_1_1Registry_1_1Type.md#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](classendstone_1_1Registry_1_1Type.md#function-operator_2) ([**const**](classendstone_1_1Identifier.md) [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](classendstone_1_1Registry_1_1Type.md#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & other) const<br> |
| virtual  | [**~Type**](classendstone_1_1Registry_1_1Type.md#function-type) () = default<br> |




## Public Static Functions inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get**](classendstone_1_1Registry_1_1Type.md#function-get) ([**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) id) <br>_Look up a registry entry by identifier._  |



















































------------------------------
The documentation for this class was generated from the following file `include/endstone/block/biome.h`

