# Benchmarks

Endstone removes several bottlenecks in the Bedrock Dedicated Server (BDS) while keeping vanilla gameplay. This page
records how it performs under a fixed heavy load, how it compares with vanilla BDS, earlier Endstone releases and other
Bedrock servers, and how the numbers were measured.

All results below use the same load: 100 players spread across a fresh world, view distance 10, simulation distance 4,
client-side chunk generation on.

## Endstone 0.11.14

### Windows

|                                     | Vanilla BDS 1.26.52 | Endstone 0.11.13 | Endstone 0.11.14 |
|-------------------------------------|--------------------:|-----------------:|-----------------:|
| Steady TPS                          |                9.40 |            11.28 |         **20.0** |
| Main thread per tick                |              105 ms |            89 ms |        **43 ms** |
| View load time, median              |               192 s |            189 s |         **81 s** |
| TPS under chaos, normal tick speed¹ |                11.8 |              8.9 |         **19.4** |
| CPU cores, steady                   |                1.07 |            1.81² |             1.04 |
| Memory (private)                    |              4.1 GB |           4.9 GB |           4.9 GB |

### Linux

|                                     | Vanilla BDS 1.26.52 | Endstone 0.11.13 | Endstone 0.11.14 |
|-------------------------------------|--------------------:|-----------------:|-----------------:|
| Steady TPS                          |                9.46 |             9.45 |         **20.0** |
| Main thread per tick                |            105.7 ms |         105.9 ms |      **46.5 ms** |
| View load time, median              |               194 s |            192 s |        **130 s** |
| TPS under chaos, normal tick speed¹ |                14.8 |             10.2 |         **19.3** |
| CPU cores, steady                   |                1.09 |             1.11 |             1.09 |
| Memory (RSS)                        |             3.96 GB |          4.06 GB |        4.54 GB³ |

¹ Measured on a 0.11.14 development build from before the packet compression and terrain generation changes.
² About 0.7 of a core is a DevTools thread that spins in 0.11.13. It is fixed in 0.11.14.
³ Endstone 0.11.14 uses mimalloc on Linux, which uses more memory than glibc malloc in exchange for speed.

When all 100 players join at the same moment, TPS drops for about 30 seconds while the server takes in the newly
generated terrain. The lowest 15-second average was 17.5 on Windows and 8.0 on Linux. Steady TPS is not affected.

## Other Bedrock servers

Windows, same machine, same load and harness. The other servers do not support client-side chunk generation, so they
send about 450 chunks to each player, where Endstone sends about 100.

| Server                            | Steady TPS | Lowest TPS while views load | CPU cores, steady |   Memory | Notes                                                    |
|-----------------------------------|-----------:|----------------------------:|------------------:|---------:|----------------------------------------------------------|
| Endstone 0.11.14                  |       20.0 |                        16.5 |              1.04 |   4.9 GB | Full vanilla gameplay                                    |
| Dragonfly (`1793369`)             |       20.0 |                        14.6 |              0.56 |   1.9 GB | Flat world, no mobs, no player movement checks           |
| Nukkit (`b3caad6`)                |       20.0 |                         0.7 |              1.32 |   5.0 GB | Pre-generated world; shut down at its first autosave on a fresh one |
| PocketMine-MP 5.44.3              |        4.6 |                         1.5 |              1.07 |   6.2 GB |                                                          |
| PowerNukkitX 3.0.5                |        1.3 |                         0.7 |              3.67 |  16.1 GB | 4.3 TPS on a pre-generated world; JVM default heap       |

None of these servers implements the locator bar or server-side player movement checks.

## Methodology

**Hardware.** Intel Core i7-14700F (8 performance cores, 12 efficiency cores), Windows 11. The server is pinned to the
performance cores (logical CPUs 0 to 15) and the bots to the efficiency cores (16 to 27), so the load generator does
not compete with the server. Linux runs use Docker on WSL2 with the same pinning.

**Server.** BDS 1.26.52.3, a fresh world with seed 20261010 for every run, `view-distance=10`, `tick-distance=4`,
`client-side-chunk-generation-enabled=true` and the default `max-threads`.

**Load.** 100 headless clients join at the same time over RakNet. Each is teleported to its own cell of a grid, far
enough apart that no two players share chunks, and stays there.

**Phases.** 20 seconds idle, the join, then a settle phase that ends when players receive fewer than 0.1 new chunks per
second each (at least 300 seconds, at most 900), then a 90-second steady window.

**Metrics.**

- TPS: game time advanced per second of wall time, from `time query gametime` every 5 seconds.
- Main thread per tick: CPU time of the server tick thread per second, divided by TPS.
- View load time: for each player, seconds from its teleport until it holds 90% of its final set of chunks. The median
  across players is reported.
- CPU cores and memory: whole process, over the steady window. Memory is private bytes on Windows and RSS on Linux.

**Chaos.** 10 to 15 minutes of randomized admin actions: joins and kicks, teleports, game mode changes, kills, summons,
block edits, weather, ticking areas, dimension changes and random tick speed changes between 0 and 1000. TPS is reported
for the time spent at normal tick speeds (0, 1 and 3), split by when the server applied each change.

**Equivalence.** An instrumented build runs vanilla's own code next to Endstone's player lookup, chunk publishing and
locator bar changes on every call during a chaos run and compares the results. It found 0 differences in 745 million
checks on Windows and 833 million on Linux. The one exception is the locator bar on Linux: vanilla Linux merges some
pairs of players because of a weak hash, and Endstone does not, so it behaves like vanilla Windows there.

**Correctness.** The in-server test suite passes 208 of 208 tests on both platforms.

## Limitations

- One machine and one workload. Redstone-heavy builds, large mob farms and more than 100 players are not covered.
- Each figure comes from a single run. Run-to-run noise on the main thread is about ±15 ms per second.
- The bots connect without encryption, so its cost is not included.
- Linux runs in Docker on WSL2, not on bare metal.

The load-test harness is not published yet.
