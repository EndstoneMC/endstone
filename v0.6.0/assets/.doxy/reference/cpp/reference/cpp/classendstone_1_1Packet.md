

# Class endstone::Packet



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Packet**](classendstone_1_1Packet.md)



_Represents a packet._ 

* `#include <endstone/network/packet.h>`





Inherited by the following classes: [endstone::SpawnParticleEffectPacket](classendstone_1_1SpawnParticleEffectPacket.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual PacketType | [**getType**](#function-gettype) () const = 0<br>_Gets the type of the packet._  |
| virtual  | [**~Packet**](#function-packet) () = default<br> |




























## Public Functions Documentation




### function getType 

_Gets the type of the packet._ 
```C++
virtual PacketType endstone::Packet::getType () const = 0
```





**Returns:**

The type of the packet. 





        

<hr>



### function ~Packet 

```C++
virtual endstone::Packet::~Packet () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/network/packet.h`

