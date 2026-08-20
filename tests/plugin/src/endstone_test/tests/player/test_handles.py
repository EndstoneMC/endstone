from endstone import Player
from endstone.actor import Actor, Mob

# =============================================================================
# Section 1: is_valid
# =============================================================================


def test_player_is_valid(player: Player) -> None:
    """Verify a connected player reports itself as valid."""
    assert player.is_valid is True


def test_is_valid_is_defined_on_actor() -> None:
    """Verify is_valid lives on Actor and is inherited rather than redefined."""
    assert "is_valid" in vars(Actor)
    assert "is_valid" not in vars(Player)
    assert "is_valid" not in vars(Mob)


def test_dimension_is_valid(player: Player) -> None:
    """Verify a loaded dimension reports itself as valid."""
    assert player.dimension.is_valid is True


# =============================================================================
# Section 2: Handles outlive the call that produced them
# =============================================================================


def test_a_looked_up_handle_stays_usable(player: Player) -> None:
    """Verify a handle from a lookup keeps working after the lookup returns."""
    handle = player.server.get_player(player.name)
    assert handle is not None
    assert handle.is_valid is True
    assert handle.name == player.name
    assert handle.unique_id == player.unique_id


def test_online_players_are_handles(player: Player) -> None:
    """Verify the online player list hands out usable handles."""
    for online in player.server.online_players:
        assert online.is_valid is True
        assert online.name


def test_dimension_actors_are_handles(player: Player) -> None:
    """Verify the actor list hands out usable handles."""
    for actor in player.dimension.actors:
        assert isinstance(actor, Actor)
        assert actor.is_valid is True


def test_the_player_appears_among_the_dimension_actors(player: Player) -> None:
    """Verify the connected player is reachable through the dimension's actors."""
    matches = [a for a in player.dimension.actors if a.runtime_id == player.runtime_id]
    assert len(matches) == 1
    assert isinstance(matches[0], Player)


# =============================================================================
# Section 3: Spawning and invalidation
# =============================================================================


def test_spawn_actor_returns_a_handle(player: Player) -> None:
    """Verify a spawned actor comes back as a usable handle."""
    actor = player.dimension.spawn_actor(player.location, "minecraft:chicken")
    try:
        assert actor is not None
        assert actor.is_valid is True
        assert actor.type == "minecraft:chicken"
    finally:
        if actor is not None and actor.is_valid:
            actor.remove()


def test_a_removed_actor_becomes_invalid(player: Player) -> None:
    """Verify a handle to a removed actor reports itself invalid instead of crashing."""
    actor = player.dimension.spawn_actor(player.location, "minecraft:chicken")
    assert actor is not None
    assert actor.is_valid is True

    actor.remove()
    assert actor.is_valid is False


def test_a_removed_actor_is_still_readable(player: Player) -> None:
    """Verify a removed actor still reads, the way CraftEntity keeps handing back its entity."""
    actor = player.dimension.spawn_actor(player.location, "minecraft:chicken")
    assert actor is not None
    actor.remove()

    assert actor.is_valid is False
    assert actor.location is not None


# =============================================================================
# Section 4: Type hierarchy after the v0.12 split
# =============================================================================


def test_player_no_longer_inherits_offline_player(player: Player) -> None:
    """Verify Player was split from OfflinePlayer in v0.12."""
    from endstone import OfflinePlayer

    assert not isinstance(player, OfflinePlayer)
    assert not issubclass(Player, OfflinePlayer)


def test_player_is_a_mob_and_an_actor(player: Player) -> None:
    """Verify a player is still a mob and an actor."""
    assert isinstance(player, Mob)
    assert isinstance(player, Actor)


# =============================================================================
# Section 5: Location holds its dimension weakly
# =============================================================================


def test_location_dimension_is_the_live_dimension(player: Player) -> None:
    """Verify a location built by the server points at the loaded dimension."""
    location = player.location
    assert location.dimension is not None
    assert location.dimension.is_valid is True
    assert location.dimension.id == player.dimension.id


def test_location_dimension_is_writable(player: Player) -> None:
    """Verify the dimension on a location can be replaced and cleared."""
    location = player.location
    location.dimension = None
    assert location.dimension is None

    location.dimension = player.dimension
    assert location.dimension.id == player.dimension.id
