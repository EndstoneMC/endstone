

# Class endstone::Mob



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Mob**](classendstone_1_1Mob.md)



_Represents a mobile entity (i.e. living entity), such as a monster or player._ 

* `#include <endstone/actor/mob.h>`



Inherits the following classes: [endstone::Actor](classendstone_1_1Actor.md)


Inherited by the following classes: [endstone::Player](classendstone_1_1Player.md)




























































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**isGliding**](#function-isgliding) () const = 0<br>_Checks to see if an actor is gliding, such as using an Elytra._  |


## Public Functions inherited from endstone::Actor

See [endstone::Actor](classendstone_1_1Actor.md)

| Type | Name |
| ---: | :--- |
|   | [**Actor**](classendstone_1_1Actor.md#function-actor-13) () = default<br> |
|   | [**Actor**](classendstone_1_1Actor.md#function-actor-23) (const [**Actor**](classendstone_1_1Actor.md) &) = delete<br> |
|   | [**Actor**](classendstone_1_1Actor.md#function-actor-33) ([**Actor**](classendstone_1_1Actor.md) &&) = delete<br> |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](classendstone_1_1Actor.md#function-getdimension) () const = 0<br> |
| virtual std::int64\_t | [**getId**](classendstone_1_1Actor.md#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1Actor.md#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](classendstone_1_1Actor.md#function-getlocation) () const = 0<br> |
| virtual std::uint64\_t | [**getRuntimeId**](classendstone_1_1Actor.md#function-getruntimeid) () const = 0<br> |
| virtual [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getVelocity**](classendstone_1_1Actor.md#function-getvelocity) () const = 0<br> |
| virtual bool | [**isDead**](classendstone_1_1Actor.md#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual bool | [**isInLava**](classendstone_1_1Actor.md#function-isinlava) () const = 0<br> |
| virtual bool | [**isInWater**](classendstone_1_1Actor.md#function-isinwater) () const = 0<br> |
| virtual bool | [**isOnGround**](classendstone_1_1Actor.md#function-isonground) () const = 0<br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**operator=**](classendstone_1_1Actor.md#function-operator) (const [**Actor**](classendstone_1_1Actor.md) &) = delete<br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**operator=**](classendstone_1_1Actor.md#function-operator_1) ([**Actor**](classendstone_1_1Actor.md) &&) = delete<br> |
| virtual void | [**setRotation**](classendstone_1_1Actor.md#function-setrotation) (float yaw, float pitch) = 0<br> |
| virtual void | [**teleport**](classendstone_1_1Actor.md#function-teleport-12) ([**Location**](classendstone_1_1Location.md) location) = 0<br> |
| virtual void | [**teleport**](classendstone_1_1Actor.md#function-teleport-22) ([**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |
|   | [**~Actor**](classendstone_1_1Actor.md#function-actor) () override<br> |


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




### function isGliding 

_Checks to see if an actor is gliding, such as using an Elytra._ 
```C++
virtual bool endstone::Mob::isGliding () const = 0
```





**Returns:**

True if this actor is gliding. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/actor/mob.h`

