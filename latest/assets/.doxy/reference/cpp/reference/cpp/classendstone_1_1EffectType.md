

# Class endstone::EffectType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EffectType**](classendstone_1_1EffectType.md)



_All effect types._ 

* `#include <endstone/effect/effect_type.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Absorption**](#variable-absorption)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("absorption")`<br>_Increases the maximum health of an entity with health that cannot be regenerated, but is refilled every 30 seconds._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**BadOmen**](#variable-badomen)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("bad\_omen")`<br>_Triggers an ominous event when the player enters a village or trial chambers._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Blindness**](#variable-blindness)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("blindness")`<br>_Blinds an entity._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**BreathOfTheNautilus**](#variable-breathofthenautilus)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("breath\_of\_the\_nautilus")`<br>_Prevents the rider's oxygen bar from depleting while riding a nautilus or zombie nautilus._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ConduitPower**](#variable-conduitpower)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("conduit\_power")`<br>_Effects granted by a nearby conduit._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Darkness**](#variable-darkness)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("darkness")`<br>_Causes the player's vision to dim occasionally._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FatalPoison**](#variable-fatalpoison)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("fatal\_poison")`<br>_Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FireResistance**](#variable-fireresistance)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("fire\_resistance")`<br>_Stops fire damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Haste**](#variable-haste)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("haste")`<br>_Increases dig speed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**HealthBoost**](#variable-healthboost)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("health\_boost")`<br>_Increases the maximum health of an entity._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**HeroOfTheVillage**](#variable-heroofthevillage)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("hero\_of\_the\_village")`<br>_Reduces the cost of villager trades._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Hunger**](#variable-hunger)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("hunger")`<br>_Increases hunger._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Infested**](#variable-infested)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("infested")`<br>_Chance of spawning silverfish when hurt._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**InstantDamage**](#variable-instantdamage)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("instant\_damage")`<br>_Hurts an entity._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**InstantHealth**](#variable-instanthealth)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("instant\_health")`<br>_Heals an entity._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Invisibility**](#variable-invisibility)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("invisibility")`<br>_Grants invisibility._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**JumpBoost**](#variable-jumpboost)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("jump\_boost")`<br>_Increases jump height._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Levitation**](#variable-levitation)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("levitation")`<br>_Causes the entity to float into the air._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**MiningFatigue**](#variable-miningfatigue)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("mining\_fatigue")`<br>_Decreases dig speed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Nausea**](#variable-nausea)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("nausea")`<br>_Warps vision on the client._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**NightVision**](#variable-nightvision)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("night\_vision")`<br>_Allows an entity to see in the dark._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Oozing**](#variable-oozing)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("oozing")`<br>_Causes slimes to spawn upon death._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Poison**](#variable-poison)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("poison")`<br>_Deals damage to an entity over time._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**RaidOmen**](#variable-raidomen)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("raid\_omen")`<br>_Triggers a raid when a player enters a village._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Regeneration**](#variable-regeneration)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("regeneration")`<br>_Regenerates health._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Resistance**](#variable-resistance)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("resistance")`<br>_Decreases damage dealt to an entity._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Saturation**](#variable-saturation)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("saturation")`<br>_Increases the food level of an entity each tick._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**SlowFalling**](#variable-slowfalling)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("slow\_falling")`<br>_Slows entity fall rate._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Slowness**](#variable-slowness)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("slowness")`<br>_Decreases movement speed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Speed**](#variable-speed)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("speed")`<br>_Increases movement speed._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Strength**](#variable-strength)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("strength")`<br>_Increases damage dealt._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**TrialOmen**](#variable-trialomen)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("trial\_omen")`<br>_Causes trial spawners to become ominous._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**WaterBreathing**](#variable-waterbreathing)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("water\_breathing")`<br>_Allows breathing underwater._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Weakness**](#variable-weakness)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("weakness")`<br>_Decreases damage dealt by an entity._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Weaving**](#variable-weaving)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("weaving")`<br>_Creates cobwebs upon death._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**WindCharged**](#variable-windcharged)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("wind\_charged")`<br>_Emits a wind burst upon death._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Wither**](#variable-wither)   = `[**EffectId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("wither")`<br>_Deals damage to an entity over time and gives the health to the shooter._  |










































## Public Static Attributes Documentation




### variable Absorption 

_Increases the maximum health of an entity with health that cannot be regenerated, but is refilled every 30 seconds._ 
```C++
constexpr auto endstone::EffectType::Absorption;
```




<hr>



### variable BadOmen 

_Triggers an ominous event when the player enters a village or trial chambers._ 
```C++
constexpr auto endstone::EffectType::BadOmen;
```




 oof. 


        

<hr>



### variable Blindness 

_Blinds an entity._ 
```C++
constexpr auto endstone::EffectType::Blindness;
```




<hr>



### variable BreathOfTheNautilus 

_Prevents the rider's oxygen bar from depleting while riding a nautilus or zombie nautilus._ 
```C++
constexpr auto endstone::EffectType::BreathOfTheNautilus;
```




<hr>



### variable ConduitPower 

_Effects granted by a nearby conduit._ 
```C++
constexpr auto endstone::EffectType::ConduitPower;
```



Includes enhanced underwater abilities. 


        

<hr>



### variable Darkness 

_Causes the player's vision to dim occasionally._ 
```C++
constexpr auto endstone::EffectType::Darkness;
```




<hr>



### variable FatalPoison 

_Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither._ 
```C++
constexpr auto endstone::EffectType::FatalPoison;
```




<hr>



### variable FireResistance 

_Stops fire damage._ 
```C++
constexpr auto endstone::EffectType::FireResistance;
```




<hr>



### variable Haste 

_Increases dig speed._ 
```C++
constexpr auto endstone::EffectType::Haste;
```




<hr>



### variable HealthBoost 

_Increases the maximum health of an entity._ 
```C++
constexpr auto endstone::EffectType::HealthBoost;
```




<hr>



### variable HeroOfTheVillage 

_Reduces the cost of villager trades._ 
```C++
constexpr auto endstone::EffectType::HeroOfTheVillage;
```




 \o/. 


        

<hr>



### variable Hunger 

_Increases hunger._ 
```C++
constexpr auto endstone::EffectType::Hunger;
```




<hr>



### variable Infested 

_Chance of spawning silverfish when hurt._ 
```C++
constexpr auto endstone::EffectType::Infested;
```




<hr>



### variable InstantDamage 

_Hurts an entity._ 
```C++
constexpr auto endstone::EffectType::InstantDamage;
```




<hr>



### variable InstantHealth 

_Heals an entity._ 
```C++
constexpr auto endstone::EffectType::InstantHealth;
```




<hr>



### variable Invisibility 

_Grants invisibility._ 
```C++
constexpr auto endstone::EffectType::Invisibility;
```




<hr>



### variable JumpBoost 

_Increases jump height._ 
```C++
constexpr auto endstone::EffectType::JumpBoost;
```




<hr>



### variable Levitation 

_Causes the entity to float into the air._ 
```C++
constexpr auto endstone::EffectType::Levitation;
```




<hr>



### variable MiningFatigue 

_Decreases dig speed._ 
```C++
constexpr auto endstone::EffectType::MiningFatigue;
```




<hr>



### variable Nausea 

_Warps vision on the client._ 
```C++
constexpr auto endstone::EffectType::Nausea;
```




<hr>



### variable NightVision 

_Allows an entity to see in the dark._ 
```C++
constexpr auto endstone::EffectType::NightVision;
```




<hr>



### variable Oozing 

_Causes slimes to spawn upon death._ 
```C++
constexpr auto endstone::EffectType::Oozing;
```




<hr>



### variable Poison 

_Deals damage to an entity over time._ 
```C++
constexpr auto endstone::EffectType::Poison;
```




<hr>



### variable RaidOmen 

_Triggers a raid when a player enters a village._ 
```C++
constexpr auto endstone::EffectType::RaidOmen;
```




<hr>



### variable Regeneration 

_Regenerates health._ 
```C++
constexpr auto endstone::EffectType::Regeneration;
```




<hr>



### variable Resistance 

_Decreases damage dealt to an entity._ 
```C++
constexpr auto endstone::EffectType::Resistance;
```




<hr>



### variable Saturation 

_Increases the food level of an entity each tick._ 
```C++
constexpr auto endstone::EffectType::Saturation;
```




<hr>



### variable SlowFalling 

_Slows entity fall rate._ 
```C++
constexpr auto endstone::EffectType::SlowFalling;
```




<hr>



### variable Slowness 

_Decreases movement speed._ 
```C++
constexpr auto endstone::EffectType::Slowness;
```




<hr>



### variable Speed 

_Increases movement speed._ 
```C++
constexpr auto endstone::EffectType::Speed;
```




<hr>



### variable Strength 

_Increases damage dealt._ 
```C++
constexpr auto endstone::EffectType::Strength;
```




<hr>



### variable TrialOmen 

_Causes trial spawners to become ominous._ 
```C++
constexpr auto endstone::EffectType::TrialOmen;
```




<hr>



### variable WaterBreathing 

_Allows breathing underwater._ 
```C++
constexpr auto endstone::EffectType::WaterBreathing;
```




<hr>



### variable Weakness 

_Decreases damage dealt by an entity._ 
```C++
constexpr auto endstone::EffectType::Weakness;
```




<hr>



### variable Weaving 

_Creates cobwebs upon death._ 
```C++
constexpr auto endstone::EffectType::Weaving;
```




<hr>



### variable WindCharged 

_Emits a wind burst upon death._ 
```C++
constexpr auto endstone::EffectType::WindCharged;
```




<hr>



### variable Wither 

_Deals damage to an entity over time and gives the health to the shooter._ 
```C++
constexpr auto endstone::EffectType::Wither;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/effect/effect_type.h`

