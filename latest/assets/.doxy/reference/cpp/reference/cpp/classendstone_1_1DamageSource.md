

# Class endstone::DamageSource



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**DamageSource**](classendstone_1_1DamageSource.md)



_Represents a source of damage._ 

* `#include <endstone/damage/damage_source.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**getActor**](#function-getactor) () const = 0<br>_Get the actor that caused the damage to occur._  |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**getDamagingActor**](#function-getdamagingactor) () const = 0<br>_Get the actor that directly caused the damage._  |
| virtual std::string\_view | [**getType**](#function-gettype) () const = 0<br>_Get the damage type._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isIndirect**](#function-isindirect) () const = 0<br>_Get if this damage is indirect._  |
| virtual  | [**~DamageSource**](#function-damagesource) () = default<br> |




























## Public Functions Documentation




### function getActor 

_Get the actor that caused the damage to occur._ 
```C++
virtual Actor * endstone::DamageSource::getActor () const = 0
```



Not to be confused with [**DamageSource::getDamagingActor()**](classendstone_1_1DamageSource.md#function-getdamagingactor), the returned actor is the actor to which the damage is ultimately attributed if the receiver is killed. If, for example, the receiver was damaged by a projectile, the shooter/thrower would be returned.




**Returns:**

an [**Actor**](classendstone_1_1Actor.md) or null 





        

<hr>



### function getDamagingActor 

_Get the actor that directly caused the damage._ 
```C++
virtual Actor * endstone::DamageSource::getDamagingActor () const = 0
```



Not to be confused with [**DamageSource::getActor()**](classendstone_1_1DamageSource.md#function-getactor), the returned actor is the actor that actually inflicted the damage. If, for example, the receiver was damaged by a projectile, the projectile would be returned.




**Returns:**

an [**Actor**](classendstone_1_1Actor.md) or null 





        

<hr>



### function getType 

_Get the damage type._ 
```C++
virtual std::string_view endstone::DamageSource::getType () const = 0
```





**Returns:**

the damage type 





        

<hr>



### function isIndirect 

_Get if this damage is indirect._ 
```C++
virtual bool endstone::DamageSource::isIndirect () const = 0
```



Damage is considered indirect if [**DamageSource::getActor()**](classendstone_1_1DamageSource.md#function-getactor) is not equal to [**DamageSource::getDamagingActor()**](classendstone_1_1DamageSource.md#function-getdamagingactor). This will be the case, for example, if a skeleton shot an arrow or a player threw a potion.




**Returns:**

true if is indirect, false otherwise. 





        

<hr>



### function ~DamageSource 

```C++
virtual endstone::DamageSource::~DamageSource () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/damage/damage_source.h`

