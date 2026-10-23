# Mob AI controls

Mob AI controls let you reduce random strolling and looking around, space out new target searches, and control
villager sleep attempts. They are disabled by default.

## Enable the controls {#configuration-and-first-setup}

Open `endstone.toml` in your server directory. The launcher adds these defaults when you start the server:

```toml
[mob-ai]
enabled = false

[mob-ai.idle]
enabled = true
player-range-blocks = 32
disable-random-stroll = true
disable-random-look-around = true

[mob-ai.target-search]
enabled = true
player-range-blocks = 32
interval-ticks = 20

[mob-ai.villagers]
sleep-failure-skip-night = false
```

Set `enabled = true` under `[mob-ai]` to turn on the controls, then restart the server.

- **Idle:** Outside `player-range-blocks`, stop random strolling and looking around on supported mobs. Set either
  `disable-*` option to `false` to keep that behavior.
- **Target search:** Outside its own `player-range-blocks`, use `interval-ticks` to space out searches for new targets.
  Existing targets and attacks continue normally.
- **Villagers:** Set `sleep-failure-skip-night = true` to skip further sleep attempts for the night after a modern
  villager's (`minecraft:villager_v2`) bed path fails. Its default of `false` keeps normal sleep attempts.

Idle and target search each have an `enabled` switch, so you can use either feature on its own. The master switch
under `[mob-ai]` controls everything, including villager sleep and per-type overrides.

Idle and target search each have their own `player-range-blocks`. A nonspectator player in the same dimension,
at or within that distance, keeps the mob's normal behavior for that feature. Distance includes vertical
separation. These settings do not change how far mobs can detect a target. Use positive, finite numbers for
ranges and an integer from 1 to 4294967295 for `interval-ticks`.

The night is measured by world time, from 12000 until the next day's 12000. A failed attempt stays blocked through
dawn and the following daytime, including a nitwit's sleep schedule across midnight. Normal sleep checks resume
in the next period; the game's own schedule and retry checks can postpone the next attempt further.
Lightweight checks continue while the game's sleep attempts and bed-path work are skipped.

Skipping starts when a search for an owned bed returns no path or an empty path, or movement toward that bed
fails. Having no owned bed, a schedule check, an accepted partial path or normal waking does not by itself start
skipping. This setting does not wake sleeping villagers or change their sleep and wake rules. Movement, bed
changes and unloading the villager do not end it early. Restarting the server clears the failed attempts.

`/time` changes within the same period keep failed attempts blocked. A later sleep check clears failures when it
observes a different period, forward or backward. Once cleared, returning to an earlier period does not restore them.

## Set options for individual mobs {#per-type-overrides}

Add `[[mob-ai.overrides]]` entries to give particular mob types different settings. For example, use a 16-block idle
range for cows and pigs:

```toml
[[mob-ai.overrides]]
types = ["minecraft:cow", "minecraft:pig"]

[mob-ai.overrides.idle]
player-range-blocks = 16
```

For skeletons, use a 16-block idle range and a separate 48-block search range with a 40-tick search interval:

```toml
[[mob-ai.overrides]]
types = ["minecraft:skeleton"]

[mob-ai.overrides.idle]
player-range-blocks = 16

[mob-ai.overrides.target-search]
enabled = true
player-range-blocks = 48
interval-ticks = 40
```

A player 32 blocks from a skeleton would keep its normal target-search behavior while idle controls could still
apply. The override's `enabled = true` also enables target-search controls for skeletons if that feature is disabled
globally. The master switch must still be on.

Use `idle`, `target-search` or `villagers` subtables with the same options as the global settings. Leave out any
option to inherit its global value. To disable a feature for selected types, set `enabled = false` in their `idle`
or `target-search` subtable. In the `villagers` subtable, `sleep-failure-skip-night = false` keeps normal sleep
attempts after a failed bed path.

For example, skip further sleep attempts for the night after a modern villager's bed path fails:

```toml
[[mob-ai.overrides]]
types = ["minecraft:villager_v2"]

[mob-ai.overrides.villagers]
sleep-failure-skip-night = true
```

Each `types` list must be nonempty and contain unique, exact identifiers; wildcards are not supported. A type can
appear in separate rules for different features, but two rules cannot configure the same feature for the same
type. Overrides cannot make unsupported mobs eligible. Mob types outside the `minecraft:` namespace are unaffected.

## Supported mobs {#capability-boundaries-and-activity-protection}

Idle controls affect mobs with random strolling or looking-around behaviors, such as cows, pigs and spiders.
Exceptions include villagers and traders, trader llamas, bosses, wardens, creakings, golems, allays, guardians and
shulkers. Special entities such as NPCs and armor stands are also excluded.

Target-search controls support zombies, husks, drowned, zombie villagers, skeletons, strays, wither skeletons,
bogged, parched, creepers and witches. Other mobs, including spiders, endermen and piglins, keep their normal target
searches. The villager sleep setting applies only to `minecraft:villager_v2`.

Mobs keep normal idle and target-search behavior while breeding, tamed or leashed, while they have a target, or
while riding another entity or carrying passengers. Mobs that were recently hurt or interacted with also keep their
normal behavior.

## Apply or disable changes {#apply-changes-disable-and-restore}

Settings are copied at startup. Restart the server after editing `endstone.toml`; `/reload` does not apply mob AI
settings. Check the startup log for `[Perf] Mob AI policies loaded`. If settings are invalid, the log reports the
error and mob AI controls stay disabled. Unknown fields, invalid values and conflicting overrides are rejected even
when the master switch is off. Malformed TOML may stop the launcher before the server starts; correct the reported
error and try again.

To disable all controls, set `enabled = false` under `[mob-ai]` and restart. You can keep your other settings and
overrides for later.

## Check your farms {#coverage-and-verification-limits}

Try these settings with your farms and mob transport systems before relying on them: changing idle movement,
target searches or villager sleep can affect how they work.
