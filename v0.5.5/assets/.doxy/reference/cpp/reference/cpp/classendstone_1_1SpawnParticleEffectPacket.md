

# Class endstone::SpawnParticleEffectPacket



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**SpawnParticleEffectPacket**](classendstone_1_1SpawnParticleEffectPacket.md)



_Represents a packet for spawning a particle effect._ 

* `#include <endstone/network/spawn_particle_effect_packet.h>`



Inherits the following classes: [endstone::Packet](classendstone_1_1Packet.md)






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::int64\_t | [**actor\_id**](#variable-actor_id)   = {-1}<br> |
|  int | [**dimension\_id**](#variable-dimension_id)  <br> |
|  std::string | [**effect\_name**](#variable-effect_name)  <br> |
|  std::optional&lt; std::string &gt; | [**molang\_variables\_json**](#variable-molang_variables_json)  <br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**position**](#variable-position)  <br> |
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual PacketType | [**getType**](#function-gettype) () override const<br>_Gets the type of the packet._  |


## Public Functions inherited from endstone::Packet

See [endstone::Packet](classendstone_1_1Packet.md)

| Type | Name |
| ---: | :--- |
| virtual PacketType | [**getType**](classendstone_1_1Packet.md#function-gettype) () const = 0<br>_Gets the type of the packet._  |
| virtual  | [**~Packet**](classendstone_1_1Packet.md#function-packet) () = default<br> |






















































## Public Attributes Documentation




### variable actor\_id 

```C++
std::int64_t endstone::SpawnParticleEffectPacket::actor_id;
```




<hr>



### variable dimension\_id 

```C++
int endstone::SpawnParticleEffectPacket::dimension_id;
```




<hr>



### variable effect\_name 

```C++
std::string endstone::SpawnParticleEffectPacket::effect_name;
```




<hr>



### variable molang\_variables\_json 

```C++
std::optional<std::string> endstone::SpawnParticleEffectPacket::molang_variables_json;
```




<hr>



### variable position 

```C++
Vector<float> endstone::SpawnParticleEffectPacket::position;
```




<hr>
## Public Functions Documentation




### function getType 

_Gets the type of the packet._ 
```C++
inline virtual PacketType endstone::SpawnParticleEffectPacket::getType () override const
```





**Returns:**

The type of the packet. 





        
Implements [*endstone::Packet::getType*](classendstone_1_1Packet.md#function-gettype)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/network/spawn_particle_effect_packet.h`

