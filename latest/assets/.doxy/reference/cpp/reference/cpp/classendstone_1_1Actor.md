

# Class endstone::Actor



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Actor**](classendstone_1_1Actor.md)



_Represents a base actor in the level._ 

* `#include <endstone/actor/actor.h>`



Inherits the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)


Inherited by the following classes: [endstone::Mob](classendstone_1_1Mob.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Actor**](#function-actor-13) () = default<br> |
|   | [**Actor**](#function-actor-23) (const [**Actor**](classendstone_1_1Actor.md) &) = delete<br> |
|   | [**Actor**](#function-actor-33) ([**Actor**](classendstone_1_1Actor.md) &&) = delete<br> |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const = 0<br> |
| virtual std::int64\_t | [**getId**](#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](#function-getlocation) () const = 0<br> |
| virtual std::uint64\_t | [**getRuntimeId**](#function-getruntimeid) () const = 0<br> |
| virtual [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getVelocity**](#function-getvelocity) () const = 0<br> |
| virtual bool | [**isDead**](#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual bool | [**isInLava**](#function-isinlava) () const = 0<br> |
| virtual bool | [**isInWater**](#function-isinwater) () const = 0<br> |
| virtual bool | [**isOnGround**](#function-isonground) () const = 0<br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**operator=**](#function-operator) (const [**Actor**](classendstone_1_1Actor.md) &) = delete<br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**operator=**](#function-operator_1) ([**Actor**](classendstone_1_1Actor.md) &&) = delete<br> |
| virtual void | [**setRotation**](#function-setrotation) (float yaw, float pitch) = 0<br> |
| virtual void | [**teleport**](#function-teleport-12) ([**Location**](classendstone_1_1Location.md) location) = 0<br> |
| virtual void | [**teleport**](#function-teleport-22) ([**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |
|   | [**~Actor**](#function-actor) () override<br> |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
|   | [**CommandSender**](classendstone_1_1CommandSender.md#function-commandsender) () = default<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) (const Message & message) const = 0<br>_Sends this sender a error message._  |
|  void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) (const Message & message) const = 0<br>_Sends this sender a message._  |
|  void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
|   | [**~CommandSender**](classendstone_1_1CommandSender.md#function-commandsender) () override<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br> |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br> |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br> |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual bool | [**isOp**](classendstone_1_1Permissible.md#function-isop) () const = 0<br>_Checks if this object is a server operator._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br> |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual void | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br> |
| virtual bool | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br> |
| virtual void | [**setOp**](classendstone_1_1Permissible.md#function-setop) (bool value) = 0<br>_Sets the operator status of this object._  |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |
















































































## Public Functions Documentation




### function Actor [1/3]

```C++
endstone::Actor::Actor () = default
```




<hr>



### function Actor [2/3]

```C++
endstone::Actor::Actor (
    const Actor &
) = delete
```




<hr>



### function Actor [3/3]

```C++
endstone::Actor::Actor (
    Actor &&
) = delete
```




<hr>



### function getDimension 


```C++
virtual Dimension & endstone::Actor::getDimension () const = 0
```



Gets the current [**Dimension**](classendstone_1_1Dimension.md) this actor resides in




**Returns:**

The current [**Dimension**](classendstone_1_1Dimension.md) this actor resides in 





        

<hr>



### function getId 

_Returns a unique id for this actor._ 
```C++
virtual std::int64_t endstone::Actor::getId () const = 0
```





**Returns:**

[**Actor**](classendstone_1_1Actor.md) id 





        

<hr>



### function getLevel 


```C++
virtual Level & endstone::Actor::getLevel () const = 0
```



Gets the current [**Level**](classendstone_1_1Level.md) this actor resides in




**Returns:**

The current [**Level**](classendstone_1_1Level.md) this actor resides in 





        

<hr>



### function getLocation 


```C++
virtual Location endstone::Actor::getLocation () const = 0
```



Gets the actor's current position




**Returns:**

a new copy of [**Location**](classendstone_1_1Location.md) containing the position of this actor 





        

<hr>



### function getRuntimeId 


```C++
virtual std::uint64_t endstone::Actor::getRuntimeId () const = 0
```



Returns the runtime id for this actor




**Returns:**

Runtime id for this actor 





        

<hr>



### function getVelocity 


```C++
virtual Vector < float > endstone::Actor::getVelocity () const = 0
```



Gets this actor's current velocity




**Returns:**

Current traveling velocity of this actor 





        

<hr>



### function isDead 

_Returns true if this actor has been marked for removal._ 
```C++
virtual bool endstone::Actor::isDead () const = 0
```





**Returns:**

True if it is dead. 





        

<hr>



### function isInLava 


```C++
virtual bool endstone::Actor::isInLava () const = 0
```



Returns true if the actor is in lava.




**Returns:**

True if the actor is in lava. 





        

<hr>



### function isInWater 


```C++
virtual bool endstone::Actor::isInWater () const = 0
```



Returns true if the actor is in water.




**Returns:**

True if the actor is in water. 





        

<hr>



### function isOnGround 


```C++
virtual bool endstone::Actor::isOnGround () const = 0
```



Returns true if the actor is supported by a block.




**Returns:**

True if actor is on ground. 





        

<hr>



### function operator= 

```C++
Actor & endstone::Actor::operator= (
    const Actor &
) = delete
```




<hr>



### function operator= 

```C++
Actor & endstone::Actor::operator= (
    Actor &&
) = delete
```




<hr>



### function setRotation 


```C++
virtual void endstone::Actor::setRotation (
    float yaw,
    float pitch
) = 0
```





**Returns:**

Sets the actor's rotation. 


Note that if the actor is affected by AI, it may override this rotation.




**Parameters:**


* `yaw` Rotation around the up axis (Y axis) 
* `pitch` Rotation around the right axis (X axis) 




        

<hr>



### function teleport [1/2]


```C++
virtual void endstone::Actor::teleport (
    Location location
) = 0
```





**Returns:**

Teleports this actor to the given location.




**Parameters:**


* `location` New location to teleport this actor to 




        

<hr>



### function teleport [2/2]


```C++
virtual void endstone::Actor::teleport (
    Actor & target
) = 0
```



Teleports this actor to the target [**Actor**](classendstone_1_1Actor.md).




**Parameters:**


* `target` [**Actor**](classendstone_1_1Actor.md) to teleport this actor to 




        

<hr>



### function ~Actor 

```C++
endstone::Actor::~Actor () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/actor/actor.h`

