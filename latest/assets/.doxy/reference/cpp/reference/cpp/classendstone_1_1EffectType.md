

# Class endstone::EffectType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EffectType**](classendstone_1_1EffectType.md)



_All effect types._ 

* `#include <endstone/effect/effect_type.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const auto | [**Absorption**](#variable-absorption)   = `getEffectType("absorption")`<br> |
|  const auto | [**BadOmen**](#variable-badomen)   = `getEffectType("bad\_omen")`<br> |
|  const auto | [**Blindness**](#variable-blindness)   = `getEffectType("blindness")`<br> |
|  const auto | [**ConduitPower**](#variable-conduitpower)   = `getEffectType("conduit\_power")`<br> |
|  const auto | [**Darkness**](#variable-darkness)   = `getEffectType("darkness")`<br> |
|  const auto | [**FatalPoison**](#variable-fatalpoison)   = `getEffectType("fatal\_poison")`<br> |
|  const auto | [**FireResistance**](#variable-fireresistance)   = `getEffectType("fire\_resistance")`<br> |
|  const auto | [**Haste**](#variable-haste)   = `getEffectType("haste")`<br> |
|  const auto | [**HealthBoost**](#variable-healthboost)   = `getEffectType("health\_boost")`<br> |
|  const auto | [**HeroOfTheVillage**](#variable-heroofthevillage)   = `getEffectType("hero\_of\_the\_village")`<br> |
|  const auto | [**Hunger**](#variable-hunger)   = `getEffectType("hunger")`<br> |
|  const auto | [**Infested**](#variable-infested)   = `getEffectType("infested")`<br> |
|  const auto | [**InstantDamage**](#variable-instantdamage)   = `getEffectType("instant\_damage")`<br> |
|  const auto | [**InstantHealth**](#variable-instanthealth)   = `getEffectType("instant\_health")`<br> |
|  const auto | [**Invisibility**](#variable-invisibility)   = `getEffectType("invisibility")`<br> |
|  const auto | [**JumpBoost**](#variable-jumpboost)   = `getEffectType("jump\_boost")`<br> |
|  const auto | [**Levitation**](#variable-levitation)   = `getEffectType("levitation")`<br> |
|  const auto | [**MiningFatigue**](#variable-miningfatigue)   = `getEffectType("mining\_fatigue")`<br> |
|  const auto | [**Nausea**](#variable-nausea)   = `getEffectType("nausea")`<br> |
|  const auto | [**NightVision**](#variable-nightvision)   = `getEffectType("night\_vision")`<br> |
|  const auto | [**Oozing**](#variable-oozing)   = `getEffectType("oozing")`<br> |
|  const auto | [**Poison**](#variable-poison)   = `getEffectType("poison")`<br> |
|  const auto | [**RaidOmen**](#variable-raidomen)   = `getEffectType("raid\_omen")`<br> |
|  const auto | [**Regeneration**](#variable-regeneration)   = `getEffectType("regeneration")`<br> |
|  const auto | [**Resistance**](#variable-resistance)   = `getEffectType("resistance")`<br> |
|  const auto | [**Saturation**](#variable-saturation)   = `getEffectType("saturation")`<br> |
|  const auto | [**SlowFalling**](#variable-slowfalling)   = `getEffectType("slow\_falling")`<br> |
|  const auto | [**Slowness**](#variable-slowness)   = `getEffectType("slowness")`<br> |
|  const auto | [**Speed**](#variable-speed)   = `getEffectType("speed")`<br> |
|  const auto | [**Strength**](#variable-strength)   = `getEffectType("strength")`<br> |
|  const auto | [**TrialOmen**](#variable-trialomen)   = `getEffectType("trial\_omen")`<br> |
|  const auto | [**WaterBreathing**](#variable-waterbreathing)   = `getEffectType("water\_breathing")`<br> |
|  const auto | [**Weakness**](#variable-weakness)   = `getEffectType("weakness")`<br> |
|  const auto | [**Weaving**](#variable-weaving)   = `getEffectType("weaving")`<br> |
|  const auto | [**WindCharged**](#variable-windcharged)   = `getEffectType("wind\_charged")`<br> |
|  const auto | [**Wither**](#variable-wither)   = `getEffectType("wither")`<br> |










































## Public Static Attributes Documentation




### variable Absorption 

```C++
const auto endstone::EffectType::Absorption;
```



Increases the maximum health of an entity with health that cannot be regenerated, but is refilled every 30 seconds. 


        

<hr>



### variable BadOmen 

```C++
const auto endstone::EffectType::BadOmen;
```



Triggers an ominous event when the player enters a village or trial chambers.
 oof. 


        

<hr>



### variable Blindness 

```C++
const auto endstone::EffectType::Blindness;
```



Blinds an entity. 


        

<hr>



### variable ConduitPower 

```C++
const auto endstone::EffectType::ConduitPower;
```



Effects granted by a nearby conduit. Includes enhanced underwater abilities. 


        

<hr>



### variable Darkness 

```C++
const auto endstone::EffectType::Darkness;
```



Causes the player's vision to dim occasionally. 


        

<hr>



### variable FatalPoison 

```C++
const auto endstone::EffectType::FatalPoison;
```



Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither 


        

<hr>



### variable FireResistance 

```C++
const auto endstone::EffectType::FireResistance;
```



Stops fire damage. 


        

<hr>



### variable Haste 

```C++
const auto endstone::EffectType::Haste;
```



Increases dig speed. 


        

<hr>



### variable HealthBoost 

```C++
const auto endstone::EffectType::HealthBoost;
```



Increases the maximum health of an entity. 


        

<hr>



### variable HeroOfTheVillage 

```C++
const auto endstone::EffectType::HeroOfTheVillage;
```



Reduces the cost of villager trades.
 \o/. 


        

<hr>



### variable Hunger 

```C++
const auto endstone::EffectType::Hunger;
```



Increases hunger. 


        

<hr>



### variable Infested 

```C++
const auto endstone::EffectType::Infested;
```



Chance of spawning silverfish when hurt. 


        

<hr>



### variable InstantDamage 

```C++
const auto endstone::EffectType::InstantDamage;
```



Hurts an entity. 


        

<hr>



### variable InstantHealth 

```C++
const auto endstone::EffectType::InstantHealth;
```



Heals an entity. 


        

<hr>



### variable Invisibility 

```C++
const auto endstone::EffectType::Invisibility;
```



Grants invisibility. 


        

<hr>



### variable JumpBoost 

```C++
const auto endstone::EffectType::JumpBoost;
```



Increases jump height. 


        

<hr>



### variable Levitation 

```C++
const auto endstone::EffectType::Levitation;
```



Causes the entity to float into the air. 


        

<hr>



### variable MiningFatigue 

```C++
const auto endstone::EffectType::MiningFatigue;
```



Decreases dig speed. 


        

<hr>



### variable Nausea 

```C++
const auto endstone::EffectType::Nausea;
```



Warps vision on the client. 


        

<hr>



### variable NightVision 

```C++
const auto endstone::EffectType::NightVision;
```



Allows an entity to see in the dark. 


        

<hr>



### variable Oozing 

```C++
const auto endstone::EffectType::Oozing;
```



Causes slimes to spawn upon death. 


        

<hr>



### variable Poison 

```C++
const auto endstone::EffectType::Poison;
```



Deals damage to an entity over time. 


        

<hr>



### variable RaidOmen 

```C++
const auto endstone::EffectType::RaidOmen;
```



Triggers a raid when a player enters a village. 


        

<hr>



### variable Regeneration 

```C++
const auto endstone::EffectType::Regeneration;
```



Regenerates health. 


        

<hr>



### variable Resistance 

```C++
const auto endstone::EffectType::Resistance;
```



Decreases damage dealt to an entity. 


        

<hr>



### variable Saturation 

```C++
const auto endstone::EffectType::Saturation;
```



Increases the food level of an entity each tick. 


        

<hr>



### variable SlowFalling 

```C++
const auto endstone::EffectType::SlowFalling;
```



Slows entity fall rate. 


        

<hr>



### variable Slowness 

```C++
const auto endstone::EffectType::Slowness;
```



Decreases movement speed. 


        

<hr>



### variable Speed 

```C++
const auto endstone::EffectType::Speed;
```



Increases movement speed. 


        

<hr>



### variable Strength 

```C++
const auto endstone::EffectType::Strength;
```



Increases damage dealt. 


        

<hr>



### variable TrialOmen 

```C++
const auto endstone::EffectType::TrialOmen;
```



Causes trial spawners to become ominous. 


        

<hr>



### variable WaterBreathing 

```C++
const auto endstone::EffectType::WaterBreathing;
```



Allows breathing underwater. 


        

<hr>



### variable Weakness 

```C++
const auto endstone::EffectType::Weakness;
```



Decreases damage dealt by an entity. 


        

<hr>



### variable Weaving 

```C++
const auto endstone::EffectType::Weaving;
```



Creates cobwebs upon death. 


        

<hr>



### variable WindCharged 

```C++
const auto endstone::EffectType::WindCharged;
```



Emits a wind burst upon death. 


        

<hr>



### variable Wither 

```C++
const auto endstone::EffectType::Wither;
```



Deals damage to an entity over time and gives the health to the shooter. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/effect/effect_type.h`

