

# Class endstone::GameRule



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**GameRule**](classendstone_1_1GameRule.md)



_Represents a game rule._ 

* `#include <endstone/game_rule.h>`



Inherits the following classes: [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)
















## Public Types inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| typedef [**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id)  <br>_The identifier type used to look up this object in its registry._  |










## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**CommandBlockOutput**](#variable-commandblockoutput)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("commandblockoutput")`<br>_Whether command blocks notify administrators when they execute commands._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**CommandBlocksEnabled**](#variable-commandblocksenabled)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("commandblocksenabled")`<br>_Whether command blocks can execute commands._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoDayLightCycle**](#variable-dodaylightcycle)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("dodaylightcycle")`<br>_Whether the day and night cycles progress._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoEntityDrops**](#variable-doentitydrops)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("doentitydrops")`<br>_Whether non-mob entities drop items._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoFireTick**](#variable-dofiretick)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("dofiretick")`<br>_Whether fire spreads._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoImmediateRespawn**](#variable-doimmediaterespawn)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("doimmediaterespawn")`<br>_Whether players immediately respawn._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoInsomnia**](#variable-doinsomnia)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("doinsomnia")`<br>_Whether players experience insomnia._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoLimitedCrafting**](#variable-dolimitedcrafting)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("dolimitedcrafting")`<br>_Whether players can craft only unlocked recipes._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoMobLoot**](#variable-domobloot)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("domobloot")`<br>_Whether mobs drop loot._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoMobSpawning**](#variable-domobspawning)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("domobspawning")`<br>_Whether mobs spawn naturally._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoTileDrops**](#variable-dotiledrops)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("dotiledrops")`<br>_Whether blocks drop items when destroyed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DoWeatherCycle**](#variable-doweathercycle)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("doweathercycle")`<br>_Whether the weather changes naturally._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DrowningDamage**](#variable-drowningdamage)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("drowningdamage")`<br>_Whether entities take drowning damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FallDamage**](#variable-falldamage)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("falldamage")`<br>_Whether entities take fall damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FireDamage**](#variable-firedamage)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("firedamage")`<br>_Whether entities take fire damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FreezeDamage**](#variable-freezedamage)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("freezedamage")`<br>_Whether entities take freezing damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FunctionCommandLimit**](#variable-functioncommandlimit)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**int**](classendstone_1_1Identifier.md)&gt;::minecraft("functioncommandlimit")`<br>_The maximum number of commands executed by a function per tick._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**KeepInventory**](#variable-keepinventory)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("keepinventory")`<br>_Whether players keep their inventory on death._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**LocatorBar**](#variable-locatorbar)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("locatorbar")`<br>_Whether the locator bar is displayed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**MaxCommandChainLength**](#variable-maxcommandchainlength)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**int**](classendstone_1_1Identifier.md)&gt;::minecraft("maxcommandchainlength")`<br>_The maximum number of chained commands executed per tick._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**MobGriefing**](#variable-mobgriefing)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("mobgriefing")`<br>_Whether mobs can modify the world._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**NaturalRegeneration**](#variable-naturalregeneration)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("naturalregeneration")`<br>_Whether players regenerate health naturally._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**PlayerWaypoints**](#variable-playerwaypoints)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**int**](classendstone_1_1Identifier.md)&gt;::minecraft("playerwaypoints")`<br>_Whether player waypoints are added to locator bars._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**PlayersSleepingPercentage**](#variable-playerssleepingpercentage)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**int**](classendstone_1_1Identifier.md)&gt;::minecraft("playerssleepingpercentage")`<br>_The percentage of players that must sleep to skip the night._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ProjectilesCanBreakBlocks**](#variable-projectilescanbreakblocks)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("projectilescanbreakblocks")`<br>_Whether projectiles can break supported blocks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Pvp**](#variable-pvp)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("pvp")`<br>_Whether players can damage each other._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**RandomTickSpeed**](#variable-randomtickspeed)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**int**](classendstone_1_1Identifier.md)&gt;::minecraft("randomtickspeed")`<br>_The frequency of random block ticks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**RecipesUnlock**](#variable-recipesunlock)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("recipesunlock")`<br>_Whether built-in recipes unlock as players progress._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**RespawnBlocksExplode**](#variable-respawnblocksexplode)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("respawnblocksexplode")`<br>_Whether respawn blocks explode in other dimensions._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**SendCommandFeedback**](#variable-sendcommandfeedback)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("sendcommandfeedback")`<br>_Whether command feedback is displayed to players._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ShowBorderEffect**](#variable-showbordereffect)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("showbordereffect")`<br>_Whether border block effects are shown._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ShowCoordinates**](#variable-showcoordinates)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("showcoordinates")`<br>_Whether player coordinates are displayed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ShowDaysPlayed**](#variable-showdaysplayed)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("showdaysplayed")`<br>_Whether the number of days played is displayed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ShowDeathMessages**](#variable-showdeathmessages)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("showdeathmessages")`<br>_Whether death messages are displayed in chat._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ShowRecipeMessages**](#variable-showrecipemessages)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("showrecipemessages")`<br>_Whether recipe unlock notifications are shown._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ShowTags**](#variable-showtags)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("showtags")`<br>_Whether item tags are shown._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**SpawnRadius**](#variable-spawnradius)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**int**](classendstone_1_1Identifier.md)&gt;::minecraft("spawnradius")`<br>_The radius around world spawn in which players can spawn._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**TntExplodes**](#variable-tntexplodes)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("tntexplodes")`<br>_Whether TNT blocks can be lit._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**TntExplosionDropDecay**](#variable-tntexplosiondropdecay)   = `[**GameRuleId**](classendstone_1_1GameRuleId.md)&lt;[**bool**](classendstone_1_1Identifier.md)&gt;::minecraft("tntexplosiondropdecay")`<br>_Whether explosion drop decay is enabled for TNT._  |






























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


















































## Public Static Attributes Documentation




### variable CommandBlockOutput 

_Whether command blocks notify administrators when they execute commands._ 
```C++
constexpr auto endstone::GameRule::CommandBlockOutput;
```




<hr>



### variable CommandBlocksEnabled 

_Whether command blocks can execute commands._ 
```C++
constexpr auto endstone::GameRule::CommandBlocksEnabled;
```




<hr>



### variable DoDayLightCycle 

_Whether the day and night cycles progress._ 
```C++
constexpr auto endstone::GameRule::DoDayLightCycle;
```




<hr>



### variable DoEntityDrops 

_Whether non-mob entities drop items._ 
```C++
constexpr auto endstone::GameRule::DoEntityDrops;
```




<hr>



### variable DoFireTick 

_Whether fire spreads._ 
```C++
constexpr auto endstone::GameRule::DoFireTick;
```




<hr>



### variable DoImmediateRespawn 

_Whether players immediately respawn._ 
```C++
constexpr auto endstone::GameRule::DoImmediateRespawn;
```




<hr>



### variable DoInsomnia 

_Whether players experience insomnia._ 
```C++
constexpr auto endstone::GameRule::DoInsomnia;
```




<hr>



### variable DoLimitedCrafting 

_Whether players can craft only unlocked recipes._ 
```C++
constexpr auto endstone::GameRule::DoLimitedCrafting;
```




<hr>



### variable DoMobLoot 

_Whether mobs drop loot._ 
```C++
constexpr auto endstone::GameRule::DoMobLoot;
```




<hr>



### variable DoMobSpawning 

_Whether mobs spawn naturally._ 
```C++
constexpr auto endstone::GameRule::DoMobSpawning;
```




<hr>



### variable DoTileDrops 

_Whether blocks drop items when destroyed._ 
```C++
constexpr auto endstone::GameRule::DoTileDrops;
```




<hr>



### variable DoWeatherCycle 

_Whether the weather changes naturally._ 
```C++
constexpr auto endstone::GameRule::DoWeatherCycle;
```




<hr>



### variable DrowningDamage 

_Whether entities take drowning damage._ 
```C++
constexpr auto endstone::GameRule::DrowningDamage;
```




<hr>



### variable FallDamage 

_Whether entities take fall damage._ 
```C++
constexpr auto endstone::GameRule::FallDamage;
```




<hr>



### variable FireDamage 

_Whether entities take fire damage._ 
```C++
constexpr auto endstone::GameRule::FireDamage;
```




<hr>



### variable FreezeDamage 

_Whether entities take freezing damage._ 
```C++
constexpr auto endstone::GameRule::FreezeDamage;
```




<hr>



### variable FunctionCommandLimit 

_The maximum number of commands executed by a function per tick._ 
```C++
constexpr auto endstone::GameRule::FunctionCommandLimit;
```




<hr>



### variable KeepInventory 

_Whether players keep their inventory on death._ 
```C++
constexpr auto endstone::GameRule::KeepInventory;
```




<hr>



### variable LocatorBar 

_Whether the locator bar is displayed._ 
```C++
constexpr auto endstone::GameRule::LocatorBar;
```




<hr>



### variable MaxCommandChainLength 

_The maximum number of chained commands executed per tick._ 
```C++
constexpr auto endstone::GameRule::MaxCommandChainLength;
```




<hr>



### variable MobGriefing 

_Whether mobs can modify the world._ 
```C++
constexpr auto endstone::GameRule::MobGriefing;
```




<hr>



### variable NaturalRegeneration 

_Whether players regenerate health naturally._ 
```C++
constexpr auto endstone::GameRule::NaturalRegeneration;
```




<hr>



### variable PlayerWaypoints 

_Whether player waypoints are added to locator bars._ 
```C++
constexpr auto endstone::GameRule::PlayerWaypoints;
```




<hr>



### variable PlayersSleepingPercentage 

_The percentage of players that must sleep to skip the night._ 
```C++
constexpr auto endstone::GameRule::PlayersSleepingPercentage;
```




<hr>



### variable ProjectilesCanBreakBlocks 

_Whether projectiles can break supported blocks._ 
```C++
constexpr auto endstone::GameRule::ProjectilesCanBreakBlocks;
```




<hr>



### variable Pvp 

_Whether players can damage each other._ 
```C++
constexpr auto endstone::GameRule::Pvp;
```




<hr>



### variable RandomTickSpeed 

_The frequency of random block ticks._ 
```C++
constexpr auto endstone::GameRule::RandomTickSpeed;
```




<hr>



### variable RecipesUnlock 

_Whether built-in recipes unlock as players progress._ 
```C++
constexpr auto endstone::GameRule::RecipesUnlock;
```




<hr>



### variable RespawnBlocksExplode 

_Whether respawn blocks explode in other dimensions._ 
```C++
constexpr auto endstone::GameRule::RespawnBlocksExplode;
```




<hr>



### variable SendCommandFeedback 

_Whether command feedback is displayed to players._ 
```C++
constexpr auto endstone::GameRule::SendCommandFeedback;
```




<hr>



### variable ShowBorderEffect 

_Whether border block effects are shown._ 
```C++
constexpr auto endstone::GameRule::ShowBorderEffect;
```




<hr>



### variable ShowCoordinates 

_Whether player coordinates are displayed._ 
```C++
constexpr auto endstone::GameRule::ShowCoordinates;
```




<hr>



### variable ShowDaysPlayed 

_Whether the number of days played is displayed._ 
```C++
constexpr auto endstone::GameRule::ShowDaysPlayed;
```




<hr>



### variable ShowDeathMessages 

_Whether death messages are displayed in chat._ 
```C++
constexpr auto endstone::GameRule::ShowDeathMessages;
```




<hr>



### variable ShowRecipeMessages 

_Whether recipe unlock notifications are shown._ 
```C++
constexpr auto endstone::GameRule::ShowRecipeMessages;
```




<hr>



### variable ShowTags 

_Whether item tags are shown._ 
```C++
constexpr auto endstone::GameRule::ShowTags;
```




<hr>



### variable SpawnRadius 

_The radius around world spawn in which players can spawn._ 
```C++
constexpr auto endstone::GameRule::SpawnRadius;
```




<hr>



### variable TntExplodes 

_Whether TNT blocks can be lit._ 
```C++
constexpr auto endstone::GameRule::TntExplodes;
```




<hr>



### variable TntExplosionDropDecay 

_Whether explosion drop decay is enabled for TNT._ 
```C++
constexpr auto endstone::GameRule::TntExplosionDropDecay;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/game_rule.h`

