

# Class endstone::Ability



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Ability**](classendstone_1_1Ability.md)



_All player abilities._ [More...](#detailed-description)

* `#include <endstone/ability.h>`



Inherits the following classes: [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)
















## Public Types inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| typedef [**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id)  <br>_The identifier type used to look up this object in its registry._  |










## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**AttackMobs**](#variable-attackmobs)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("attackmobs")`<br>_Whether the player can attack mobs._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**AttackPlayers**](#variable-attackplayers)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("attackplayers")`<br>_Whether the player can attack other players._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Build**](#variable-build)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("build")`<br>_Whether the player can place blocks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoorsAndSwitches**](#variable-doorsandswitches)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("doorsandswitches")`<br>_Whether the player can use doors, trapdoors, buttons, levers and other redstone components._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FlySpeed**](#variable-flyspeed)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**float**](classendstone_1_1Identifier.md)&gt;::minecraft("flyspeed")`<br>_The speed at which the player flies, default is_ `0.05` _._ |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Flying**](#variable-flying)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("flying")`<br>_Whether the player is currently flying._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Instabuild**](#variable-instabuild)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("instabuild")`<br>_Whether the player destroys blocks instantly._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Invulnerable**](#variable-invulnerable)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("invulnerable")`<br>_Whether the player is immune to all damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Lightning**](#variable-lightning)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("lightning")`<br>_Whether the player was struck by lightning._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**MayFly**](#variable-mayfly)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("mayfly")`<br>_Whether the player is allowed to fly._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Mine**](#variable-mine)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("mine")`<br>_Whether the player can destroy blocks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Muted**](#variable-muted)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("mute")`<br>_Whether the player's chat messages are hidden from other players._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**NoClip**](#variable-noclip)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("noclip")`<br>_Whether the player can move through blocks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**OpenContainers**](#variable-opencontainers)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("opencontainers")`<br>_Whether the player can open containers._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**OperatorCommands**](#variable-operatorcommands)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("op")`<br>_Whether the player can use operator commands._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**PrivilegedBuilder**](#variable-privilegedbuilder)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("privilegedbuilder")`<br>_Whether the player is a privileged builder._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Teleport**](#variable-teleport)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("teleport")`<br>_Whether the player can teleport._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**VerticalFlySpeed**](#variable-verticalflyspeed)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**float**](classendstone_1_1Identifier.md)&gt;::minecraft("verticalflyspeed")`<br>_The speed at which the player flies up and down, default is_ `1` _._ |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**WalkSpeed**](#variable-walkspeed)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**float**](classendstone_1_1Identifier.md)&gt;::minecraft("walkspeed")`<br>_The speed at which the player walks, default is_ `0.1` _._ |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**WorldBuilder**](#variable-worldbuilder)   = `[**AbilityId**](classendstone_1_1AbilityId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("worldbuilder")`<br>_Whether the player is a world builder._  |






























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


















































## Detailed Description


AttackMobs, AttackPlayers, Build, DoorsAndSwitches, Mine, OpenContainers, OperatorCommands and Teleport are the eight member permissions the client shows in its pause menu, and the server enforces every one of them. 


    
## Public Static Attributes Documentation




### variable AttackMobs 

_Whether the player can attack mobs._ 
```C++
constexpr auto endstone::Ability::AttackMobs;
```




<hr>



### variable AttackPlayers 

_Whether the player can attack other players._ 
```C++
constexpr auto endstone::Ability::AttackPlayers;
```




<hr>



### variable Build 

_Whether the player can place blocks._ 
```C++
constexpr auto endstone::Ability::Build;
```




<hr>



### variable DoorsAndSwitches 

_Whether the player can use doors, trapdoors, buttons, levers and other redstone components._ 
```C++
constexpr auto endstone::Ability::DoorsAndSwitches;
```




<hr>



### variable FlySpeed 

_The speed at which the player flies, default is_ `0.05` _._
```C++
constexpr auto endstone::Ability::FlySpeed;
```




<hr>



### variable Flying 

_Whether the player is currently flying._ 
```C++
constexpr auto endstone::Ability::Flying;
```




<hr>



### variable Instabuild 

_Whether the player destroys blocks instantly._ 
```C++
constexpr auto endstone::Ability::Instabuild;
```




<hr>



### variable Invulnerable 

_Whether the player is immune to all damage._ 
```C++
constexpr auto endstone::Ability::Invulnerable;
```




<hr>



### variable Lightning 

_Whether the player was struck by lightning._ 
```C++
constexpr auto endstone::Ability::Lightning;
```




<hr>



### variable MayFly 

_Whether the player is allowed to fly._ 
```C++
constexpr auto endstone::Ability::MayFly;
```




<hr>



### variable Mine 

_Whether the player can destroy blocks._ 
```C++
constexpr auto endstone::Ability::Mine;
```




<hr>



### variable Muted 

_Whether the player's chat messages are hidden from other players._ 
```C++
constexpr auto endstone::Ability::Muted;
```




<hr>



### variable NoClip 

_Whether the player can move through blocks._ 
```C++
constexpr auto endstone::Ability::NoClip;
```




<hr>



### variable OpenContainers 

_Whether the player can open containers._ 
```C++
constexpr auto endstone::Ability::OpenContainers;
```




<hr>



### variable OperatorCommands 

_Whether the player can use operator commands._ 
```C++
constexpr auto endstone::Ability::OperatorCommands;
```




<hr>



### variable PrivilegedBuilder 

_Whether the player is a privileged builder._ 
```C++
constexpr auto endstone::Ability::PrivilegedBuilder;
```




<hr>



### variable Teleport 

_Whether the player can teleport._ 
```C++
constexpr auto endstone::Ability::Teleport;
```




<hr>



### variable VerticalFlySpeed 

_The speed at which the player flies up and down, default is_ `1` _._
```C++
constexpr auto endstone::Ability::VerticalFlySpeed;
```




<hr>



### variable WalkSpeed 

_The speed at which the player walks, default is_ `0.1` _._
```C++
constexpr auto endstone::Ability::WalkSpeed;
```




<hr>



### variable WorldBuilder 

_Whether the player is a world builder._ 
```C++
constexpr auto endstone::Ability::WorldBuilder;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ability.h`

