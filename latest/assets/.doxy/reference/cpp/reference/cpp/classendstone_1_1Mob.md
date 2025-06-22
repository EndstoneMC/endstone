

# Class endstone::Mob



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Mob**](classendstone_1_1Mob.md)



_Represents a mobile entity (i.e. living entity), such as a monster or player._ 

* `#include <endstone/actor/mob.h>`



Inherits the following classes: [endstone::Actor](classendstone_1_1Actor.md)


Inherited by the following classes: [endstone::Player](classendstone_1_1Player.md)




























































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](#function-asmob) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual bool | [**isGliding**](#function-isgliding) () const = 0<br>_Checks to see if an actor is gliding, such as using an Elytra._  |


## Public Functions inherited from endstone::Actor

See [endstone::Actor](classendstone_1_1Actor.md)

| Type | Name |
| ---: | :--- |
| virtual bool | [**addScoreboardTag**](classendstone_1_1Actor.md#function-addscoreboardtag) (std::string tag) const = 0<br>_Adds a tag to this actor._  |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1Actor.md#function-asactor) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](classendstone_1_1Actor.md#function-getdimension) () const = 0<br> |
| virtual int | [**getHealth**](classendstone_1_1Actor.md#function-gethealth) () const = 0<br>_Gets the entity's health from 0 to its max possible value, where 0 is dead._  |
| virtual std::int64\_t | [**getId**](classendstone_1_1Actor.md#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1Actor.md#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](classendstone_1_1Actor.md#function-getlocation) () const = 0<br> |
| virtual int | [**getMaxHealth**](classendstone_1_1Actor.md#function-getmaxhealth) () const = 0<br>_Gets the maximum health this entity has._  |
| virtual std::string | [**getNameTag**](classendstone_1_1Actor.md#function-getnametag) () const = 0<br>_Gets the current name tag of the actor._  |
| virtual std::uint64\_t | [**getRuntimeId**](classendstone_1_1Actor.md#function-getruntimeid) () const = 0<br> |
| virtual std::string | [**getScoreTag**](classendstone_1_1Actor.md#function-getscoretag) () const = 0<br>_Gets the current score tag of the actor._  |
| virtual std::vector&lt; std::string &gt; | [**getScoreboardTags**](classendstone_1_1Actor.md#function-getscoreboardtags) () const = 0<br>_Returns a list of scoreboard tags for this actor._  |
| virtual std::string | [**getType**](classendstone_1_1Actor.md#function-gettype) () const = 0<br>_Get the type of the actor._  |
| virtual [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getVelocity**](classendstone_1_1Actor.md#function-getvelocity) () const = 0<br> |
| virtual bool | [**isDead**](classendstone_1_1Actor.md#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual bool | [**isInLava**](classendstone_1_1Actor.md#function-isinlava) () const = 0<br> |
| virtual bool | [**isInWater**](classendstone_1_1Actor.md#function-isinwater) () const = 0<br> |
| virtual bool | [**isNameTagAlwaysVisible**](classendstone_1_1Actor.md#function-isnametagalwaysvisible) () const = 0<br>_Checks if the actor's name tag is always visible._  |
| virtual bool | [**isNameTagVisible**](classendstone_1_1Actor.md#function-isnametagvisible) () const = 0<br>_Checks if the actor's name tag is currently visible._  |
| virtual bool | [**isOnGround**](classendstone_1_1Actor.md#function-isonground) () const = 0<br> |
| virtual bool | [**isValid**](classendstone_1_1Actor.md#function-isvalid) () const = 0<br> |
| virtual void | [**remove**](classendstone_1_1Actor.md#function-remove) () = 0<br> |
| virtual bool | [**removeScoreboardTag**](classendstone_1_1Actor.md#function-removescoreboardtag) (std::string tag) const = 0<br>_Removes a given tag from this actor._  |
| virtual Result&lt; void &gt; | [**setHealth**](classendstone_1_1Actor.md#function-sethealth) (int health) const = 0<br> |
| virtual void | [**setNameTag**](classendstone_1_1Actor.md#function-setnametag) (std::string name) = 0<br>_Sets the name tag for the actor._  |
| virtual void | [**setNameTagAlwaysVisible**](classendstone_1_1Actor.md#function-setnametagalwaysvisible) (bool visible) = 0<br>_Sets whether the actor's name tag should always be visible._  |
| virtual void | [**setNameTagVisible**](classendstone_1_1Actor.md#function-setnametagvisible) (bool visible) = 0<br>_Sets if the actor's name tag is visible or not._  |
| virtual void | [**setRotation**](classendstone_1_1Actor.md#function-setrotation) (float yaw, float pitch) = 0<br> |
| virtual void | [**setScoreTag**](classendstone_1_1Actor.md#function-setscoretag) (std::string score) = 0<br>_Sets the score tag for the actor._  |
| virtual void | [**teleport**](classendstone_1_1Actor.md#function-teleport-12) ([**Location**](classendstone_1_1Location.md) location) = 0<br> |
| virtual void | [**teleport**](classendstone_1_1Actor.md#function-teleport-22) ([**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1CommandSender.md#function-asactor) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](classendstone_1_1CommandSender.md#function-asmob) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) (const Message & message) const = 0<br>_Sends this sender a error message._  |
|  void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) (const Message & message) const = 0<br>_Sends this sender a message._  |
|  void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br>_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._ |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br>_Gets a set containing all the permissions currently in effect by this object._  |
| virtual PermissionLevel | [**getPermissionLevel**](classendstone_1_1Permissible.md#function-getpermissionlevel) () const = 0<br>_Gets the permission level of this object._  |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br>_Checks if this object contains an override for the specified permission, by fully qualified name._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
| virtual void | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br>_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._  |
| virtual Result&lt; void &gt; | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br>_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._ |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |










































































































## Public Functions Documentation




### function asMob 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._
```C++
inline virtual Mob * endstone::Mob::asMob () override const
```





**Returns:**

[**Mob**](classendstone_1_1Mob.md), nullptr if not an [**Mob**](classendstone_1_1Mob.md) 





        
Implements [*endstone::CommandSender::asMob*](classendstone_1_1CommandSender.md#function-asmob)


<hr>



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

