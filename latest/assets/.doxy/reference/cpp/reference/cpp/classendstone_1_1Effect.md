

# Class endstone::Effect



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Effect**](classendstone_1_1Effect.md)



_Represents an effect that can be added to a_ [_**Mob**_](classendstone_1_1Mob.md) _._[More...](#detailed-description)

* `#include <endstone/potion/effect.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Effect**](#function-effect) ([**EffectId**](classendstone_1_1Identifier.md) type, std::optional&lt; [**int**](classendstone_1_1Identifier.md) &gt; duration, [**int**](classendstone_1_1Identifier.md) amplifier, [**bool**](classendstone_1_1Identifier.md) ambient=[**false**](classendstone_1_1Identifier.md), [**bool**](classendstone_1_1Identifier.md) particles=[**true**](classendstone_1_1Identifier.md), [**bool**](classendstone_1_1Identifier.md) icon=[**true**](classendstone_1_1Identifier.md)) <br>_Creates an effect._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) | [**getAmplifier**](#function-getamplifier) () noexcept const<br>_Gets the amplifier of this effect._  |
|  [**constexpr**](classendstone_1_1Identifier.md) std::optional&lt; [**int**](classendstone_1_1Identifier.md) &gt; | [**getDuration**](#function-getduration) () noexcept const<br>_Gets the duration of this effect, in ticks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**EffectId**](classendstone_1_1Identifier.md) | [**getType**](#function-gettype) () noexcept const<br>_Gets the type of this effect._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**hasIcon**](#function-hasicon) () noexcept const<br>_Gets whether this effect has an icon._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**hasParticles**](#function-hasparticles) () noexcept const<br>_Gets whether this effect has particles._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isAmbient**](#function-isambient) () noexcept const<br>_Makes the effect produce more, translucent, particles._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isInfinite**](#function-isinfinite) () noexcept const<br>_Returns whether this effect has an infinite duration._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Effect**](classendstone_1_1Effect.md) & other) noexcept const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**Effect**](classendstone_1_1Effect.md) & other) noexcept const<br> |




























## Detailed Description


An effect is an immutable description of an effect type together with a duration, amplifier and a set of display options. Applying it to a [**Mob**](classendstone_1_1Mob.md) (e.g. via [**Mob::addEffect**](classendstone_1_1Mob.md#function-addeffect)) installs the effect; querying a [**Mob**](classendstone_1_1Mob.md)'s active effects yields [**Effect**](classendstone_1_1Effect.md) values describing what is currently in force. 


    
## Public Functions Documentation




### function Effect 

_Creates an effect._ 
```C++
inline constexpr endstone::Effect::Effect (
    EffectId type,
    std::optional< int > duration,
    int amplifier,
    bool ambient=false,
    bool particles=true,
    bool icon=true
) 
```





**Parameters:**


* `type` effect type 
* `duration` measured in ticks, or std::nullopt for an infinite duration, see [**Effect::getDuration**](classendstone_1_1Effect.md#function-getduration) 
* `amplifier` the amplifier for the effect, see [**Effect::getAmplifier**](classendstone_1_1Effect.md#function-getamplifier) 
* `ambient` the ambient status, see [**Effect::isAmbient**](classendstone_1_1Effect.md#function-isambient) 
* `particles` the particle status, see [**Effect::hasParticles**](classendstone_1_1Effect.md#function-hasparticles) 
* `icon` the icon status, see [**Effect::hasIcon**](classendstone_1_1Effect.md#function-hasicon) 




        

<hr>



### function getAmplifier 

_Gets the amplifier of this effect._ 
```C++
inline constexpr  int endstone::Effect::getAmplifier () noexcept const
```



A higher amplifier means the effect happens more often over its duration and in some cases has more effect on its target.




**Returns:**

the amplifier 





        

<hr>



### function getDuration 

_Gets the duration of this effect, in ticks._ 
```C++
inline constexpr std::optional< int > endstone::Effect::getDuration () noexcept const
```





**Returns:**

the duration in ticks, or std::nullopt if this effect is infinite 





        

<hr>



### function getType 

_Gets the type of this effect._ 
```C++
inline constexpr  EffectId endstone::Effect::getType () noexcept const
```





**Returns:**

the type 





        

<hr>



### function hasIcon 

_Gets whether this effect has an icon._ 
```C++
inline constexpr  bool endstone::Effect::hasIcon () noexcept const
```





**Returns:**

whether this effect has an icon 





        

<hr>



### function hasParticles 

_Gets whether this effect has particles._ 
```C++
inline constexpr  bool endstone::Effect::hasParticles () noexcept const
```





**Returns:**

whether this effect has particles 





        

<hr>



### function isAmbient 

_Makes the effect produce more, translucent, particles._ 
```C++
inline constexpr  bool endstone::Effect::isAmbient () noexcept const
```





**Returns:**

whether this effect is ambient 





        

<hr>



### function isInfinite 

_Returns whether this effect has an infinite duration._ 
```C++
inline constexpr  bool endstone::Effect::isInfinite () noexcept const
```



Effects with an infinite duration never expire unless manually removed.




**Returns:**

whether this effect is infinite 





        

<hr>



### function operator!= 

```C++
inline constexpr  bool endstone::Effect::operator!= (
    const  Effect & other
) noexcept const
```




<hr>



### function operator== 

```C++
inline constexpr  bool endstone::Effect::operator== (
    const  Effect & other
) noexcept const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/potion/effect.h`

