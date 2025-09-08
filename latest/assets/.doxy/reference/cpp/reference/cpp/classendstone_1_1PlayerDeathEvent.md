

# Class endstone::PlayerDeathEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerDeathEvent**](classendstone_1_1PlayerDeathEvent.md)



_Called when a_ [_**Player**_](classendstone_1_1Player.md) _dies._

* `#include <endstone/event/player/player_death_event.h>`



Inherits the following classes: [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md),  [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)




















































## Public Static Attributes inherited from endstone::ActorDeathEvent

See [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md)

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](classendstone_1_1ActorDeathEvent.md#variable-name)   = `"ActorDeathEvent"`<br> |


















































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerDeathEvent**](classendstone_1_1PlayerDeathEvent.md)) <br> |
|   | [**PlayerDeathEvent**](#function-playerdeathevent) ([**Player**](classendstone_1_1Player.md) & player, std::unique\_ptr&lt; [**DamageSource**](classendstone_1_1DamageSource.md) &gt; damage\_source, std::optional&lt; Message &gt; death\_message) <br> |
|  std::optional&lt; Message &gt; | [**getDeathMessage**](#function-getdeathmessage) () const<br>_Get the death message that will appear to everyone on the server._  |
|  void | [**setDeathMessage**](#function-setdeathmessage) (std::optional&lt; Message &gt; death\_message) <br>_Set the death message that will appear to everyone on the server._  |


## Public Functions inherited from endstone::ActorDeathEvent

See [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md#function-actordeathevent) ([**Mob**](classendstone_1_1Mob.md) & actor, std::unique\_ptr&lt; [**DamageSource**](classendstone_1_1DamageSource.md) &gt; damage\_source) <br> |
|  [**DamageSource**](classendstone_1_1DamageSource.md) & | [**getDamageSource**](classendstone_1_1ActorDeathEvent.md#function-getdamagesource) () const<br>_Gets the source of damage which caused the death._  |
| virtual std::string | [**getEventName**](classendstone_1_1ActorDeathEvent.md#function-geteventname) () override const<br> |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) (ActorType & actor) <br> |
|  ActorType & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br> |
|   | [**~PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |














































## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |
















































































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerDeathEvent::ENDSTONE_EVENT (
    PlayerDeathEvent
) 
```




<hr>



### function PlayerDeathEvent 

```C++
inline explicit endstone::PlayerDeathEvent::PlayerDeathEvent (
    Player & player,
    std::unique_ptr< DamageSource > damage_source,
    std::optional< Message > death_message
) 
```




<hr>



### function getDeathMessage 

_Get the death message that will appear to everyone on the server._ 
```C++
inline std::optional< Message > endstone::PlayerDeathEvent::getDeathMessage () const
```





**Returns:**

Message to appear to other players on the server. 





        

<hr>



### function setDeathMessage 

_Set the death message that will appear to everyone on the server._ 
```C++
inline void endstone::PlayerDeathEvent::setDeathMessage (
    std::optional< Message > death_message
) 
```





**Parameters:**


* `death_message` Message to appear to other players on the server. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_death_event.h`

