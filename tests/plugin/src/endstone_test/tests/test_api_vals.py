import math
import uuid
from datetime import datetime, timezone

import pytest
from endstone import Ability, GameRule, Server
from endstone.ban import IpBanEntry, PlayerBanEntry
from endstone.level import Dimension, Location
from endstone.map import MapCursor
from endstone.nbt import ByteTag, CompoundTag, IntTag, ListTag, StringTag
from endstone.util import SocketAddress, Vector


def approx(*values):
    return pytest.approx(values, abs=1e-5)


def xyz(v):
    return (v.x, v.y, v.z)


@pytest.mark.parametrize(
    "cls, constant, other",
    [(Ability, Ability.NO_CLIP, Ability.FLYING), (GameRule, GameRule.PVP, GameRule.DO_FIRE_TICK)],
    ids=["Ability", "GameRule"],
)
def test_registry_value_identity(server: Server, cls, constant, other):
    value = cls.get(constant)
    twin = cls.get(str(constant))
    different = cls.get(other)
    assert value is not None and twin is not None and different is not None
    assert str(value.id) == str(constant)

    assert value == twin
    assert not value != twin
    assert value != different
    assert not value == different
    assert value == str(constant)
    assert not value != str(constant)
    assert value != str(other)
    assert not value == str(other)

    assert hash(value) == hash(twin)
    assert len({value, twin, different}) == 2
    assert {value: 1}[twin] == 1

    assert str(constant) in str(value)
    assert str(constant) in repr(value)
    assert str(value) != str(different)

    assert cls.get("minecraft:not_a_real_entry") is None


def test_ability_get_bare_name_defaults_to_minecraft(server: Server):
    assert Ability.get("noclip") == Ability.get("minecraft:noclip")
    assert hash(Ability.get("noclip")) == hash(Ability.get("minecraft:noclip"))


def test_ip_ban_entry_init():
    before = datetime.now(timezone.utc)
    entry = IpBanEntry("203.0.113.9")
    after = datetime.now(timezone.utc)
    assert entry.address == "203.0.113.9"
    assert entry.expiration is None
    assert entry.reason
    assert entry.source
    created = entry.created if entry.created.tzinfo else entry.created.astimezone()
    assert before.replace(microsecond=0) <= created <= after
    assert IpBanEntry("203.0.113.10").address == "203.0.113.10"


def test_player_ban_entry_init():
    bare = PlayerBanEntry("Steve")
    assert bare.name == "Steve"
    assert bare.unique_id is None
    assert bare.xuid is None
    assert bare.expiration is None
    assert bare.reason
    assert bare.source

    unique_id = uuid.uuid4()
    full = PlayerBanEntry("Alex", unique_id, "2535400000000001")
    assert full.name == "Alex"
    assert full.unique_id == unique_id
    assert full.xuid == "2535400000000001"

    keyword = PlayerBanEntry("Kit", xuid="42")
    assert keyword.unique_id is None
    assert keyword.xuid == "42"


def test_socket_address_init_and_str():
    address = SocketAddress("127.0.0.1", 19132)
    assert address.hostname == "127.0.0.1"
    assert address.port == 19132
    assert str(address) == "127.0.0.1:19132"
    assert str(SocketAddress("play.example.net", 25565)) == "play.example.net:25565"

    empty = SocketAddress()
    assert empty.hostname == ""
    assert empty.port == 0
    assert str(empty) == ":0"
    assert str(SocketAddress("localhost", 1)) == "localhost:1"


def test_socket_address_rejects_negative_port():
    with pytest.raises(TypeError):
        SocketAddress("127.0.0.1", -1)


def test_map_cursor_init_and_properties():
    cursor = MapCursor(3, -4, 7, MapCursor.Type.RED_MARKER, True, "spot")
    assert (cursor.x, cursor.y, cursor.direction) == (3, -4, 7)
    assert cursor.type == MapCursor.Type.RED_MARKER
    assert cursor.is_visible is True
    assert cursor.caption == "spot"

    assert MapCursor(0, 0, 0, MapCursor.Type.PLAYER, False).caption == ""
    assert MapCursor(0, 0, 0, MapCursor.Type.PLAYER, False).is_visible is False

    cursor.x = 127
    cursor.y = -128
    cursor.direction = 15
    cursor.type = MapCursor.Type.MANSION
    cursor.is_visible = False
    cursor.caption = ""
    assert (cursor.x, cursor.y, cursor.direction) == (127, -128, 15)
    assert cursor.type == MapCursor.Type.MANSION
    assert cursor.is_visible is False
    assert cursor.caption == ""


def test_map_cursor_instances_are_independent():
    first = MapCursor(1, 1, 1, MapCursor.Type.PLAYER, True)
    second = MapCursor(1, 1, 1, MapCursor.Type.PLAYER, True)
    first.x = 9
    first.caption = "changed"
    assert second.x == 1
    assert second.caption == ""


@pytest.mark.parametrize("field, value", [("x", 128), ("x", -129), ("y", 128), ("y", -129)])
def test_map_cursor_coordinate_out_of_int8_range(field, value):
    cursor = MapCursor(0, 0, 0, MapCursor.Type.PLAYER, True)
    with pytest.raises(TypeError):
        setattr(cursor, field, value)
    assert getattr(cursor, field) == 0


@pytest.mark.parametrize("direction", [-1, 16])
def test_map_cursor_direction_out_of_range(direction):
    cursor = MapCursor(0, 0, 5, MapCursor.Type.PLAYER, True)
    with pytest.raises(ValueError):
        cursor.direction = direction
    assert cursor.direction == 5


@pytest.mark.parametrize("direction", [-1, 16])
def test_map_cursor_init_rejects_direction_out_of_range(direction):
    with pytest.raises(ValueError):
        MapCursor(0, 0, direction, MapCursor.Type.PLAYER, True)


def test_vector_repr_str_and_eq():
    v = Vector(1, 2, 3)
    assert str(v) == "1,2,3"
    assert repr(v) == "Vector(x=1, y=2, z=3)"
    assert str(Vector(0.5, -1.5, 0)) == "0.5,-1.5,0"
    assert str(Vector()) == "0,0,0"

    assert v == Vector(1, 2, 3)
    assert not v != Vector(1, 2, 3)
    assert v != Vector(1, 2, 4)
    assert not v == Vector(1, 2, 4)
    assert v == Vector(1 + 1e-8, 2, 3)
    assert v != Vector(1.001, 2, 3)
    with pytest.raises(TypeError):
        hash(v)


def test_vector_arithmetic_between_vectors_is_componentwise_and_pure():
    a = Vector(1, 2, 3)
    b = Vector(4, 5, 6)
    assert xyz(a + b) == (5, 7, 9)
    assert xyz(a - b) == (-3, -3, -3)
    assert xyz(a * b) == (4, 10, 18)
    assert xyz(a / b) == approx(0.25, 0.4, 0.5)
    assert xyz(a) == (1, 2, 3)
    assert xyz(b) == (4, 5, 6)
    assert (a @ b) == 32
    assert (a @ b) == a.dot(b)
    assert (Vector(1, 0, 0) @ Vector(0, 1, 0)) == 0


def test_vector_arithmetic_with_scalars():
    v = Vector(1, 2, 3)
    assert xyz(v + 1) == (2, 3, 4)
    assert xyz(1 + v) == (2, 3, 4)
    assert xyz(v - 1) == (0, 1, 2)
    assert xyz(2 - v) == (1, 0, -1)
    assert xyz(v * 2) == (2, 4, 6)
    assert xyz(2 * v) == (2, 4, 6)
    assert xyz(v / 2) == (0.5, 1, 1.5)
    assert xyz(6 / v) == (6, 3, 2)
    assert xyz(v) == (1, 2, 3)


def test_vector_division_by_zero_follows_ieee():
    q = Vector(1, -1, 0) / Vector(0, 0, 0)
    assert q.x == math.inf and q.y == -math.inf and math.isnan(q.z)
    q = Vector(1, -1, 2) / 0
    assert (q.x, q.y, q.z) == (math.inf, -math.inf, math.inf)


def test_vector_in_place_arithmetic_mutates_and_returns_self():
    a = Vector(1, 2, 3)
    alias = a
    a += Vector(1, 1, 1)
    assert a is alias and xyz(alias) == (2, 3, 4)
    a -= Vector(2, 2, 2)
    assert a is alias and xyz(alias) == (0, 1, 2)
    a *= Vector(3, 3, 3)
    assert a is alias and xyz(alias) == (0, 3, 6)
    a /= Vector(3, 3, 3)
    assert a is alias and xyz(alias) == (0, 1, 2)


def test_vector_component_setters():
    v = Vector()
    v.x = 1.5
    v.y = -2.5
    v.z = 3.5
    assert xyz(v) == (1.5, -2.5, 3.5)
    assert v == Vector(1.5, -2.5, 3.5)
    assert v.length_squared == pytest.approx(1.5**2 + 2.5**2 + 3.5**2)
    assert (v.block_x, v.block_y, v.block_z) == (1, -3, 3)
    v.y = 0
    assert v.is_zero is False
    assert (v.block_x, v.block_y, v.block_z) == (1, 0, 3)


def test_vector_floored_block_components():
    v = Vector(-1.5, 2.5, 3.9)
    assert (v.block_x, v.block_y, v.block_z) == (-2, 2, 3)
    assert (Vector(-0.1, 0, 0).block_x, Vector(0, 0, 0).block_x) == (-1, 0)


def test_vector_length_and_distance():
    v = Vector(3, 4, 12)
    assert v.length == pytest.approx(13)
    assert v.length_squared == pytest.approx(169)
    assert Vector().length == 0
    a = Vector(1, 2, 3)
    b = Vector(4, 5, 6)
    assert a.distance(b) == pytest.approx(math.sqrt(27))
    assert a.distance_squared(b) == pytest.approx(27)
    assert a.distance(b) == b.distance(a)
    assert a.distance(a) == 0


def test_vector_angle():
    assert Vector(1, 0, 0).angle(Vector(0, 1, 0)) == pytest.approx(math.pi / 2, abs=1e-6)
    assert Vector(1, 0, 0).angle(Vector(-1, 0, 0)) == pytest.approx(math.pi, abs=1e-6)
    assert Vector(1, 0, 0).angle(Vector(5, 0, 0)) == pytest.approx(0, abs=1e-6)
    assert Vector(1, 0, 0).angle(Vector(1, 1, 0)) == pytest.approx(math.pi / 4, abs=1e-6)
    assert math.isnan(Vector(1, 0, 0).angle(Vector()))


def test_vector_cross_product_is_pure_and_anticommutative():
    a = Vector(1, 2, 3)
    b = Vector(4, 5, 6)
    assert xyz(a.cross_product(b)) == (-3, 6, -3)
    assert xyz(b.cross_product(a)) == (3, -6, 3)
    assert xyz(a) == (1, 2, 3)
    assert xyz(Vector(1, 0, 0).cross_product(Vector(0, 1, 0))) == (0, 0, 1)
    assert a.cross_product(a).is_zero


def test_vector_midpoint_is_pure():
    a = Vector(0, 2, -4)
    b = Vector(4, 4, 4)
    mid = a.midpoint(b)
    assert xyz(mid) == (2, 3, 0)
    assert xyz(a) == (0, 2, -4)
    assert xyz(b) == (4, 4, 4)


def test_vector_normalize_mutates_and_returns_self():
    v = Vector(3, 4, 0)
    assert not v.is_normalized
    assert v.normalize() is v
    assert xyz(v) == approx(0.6, 0.8, 0)
    assert v.is_normalized
    assert v.length == pytest.approx(1, abs=1e-6)
    assert Vector(1, 0, 0).is_normalized
    assert not Vector(1.01, 0, 0).is_normalized
    assert not Vector().is_normalized


def test_vector_normalize_zero_vector_is_nan():
    v = Vector().normalize()
    assert math.isnan(v.x) and math.isnan(v.y) and math.isnan(v.z)
    assert not v.is_zero


def test_vector_normalize_zero_clears_negative_zero():
    v = Vector(-0.0, -0.0, 1)
    assert math.copysign(1, v.x) == -1
    assert v.normalize_zero() is v
    assert math.copysign(1, v.x) == 1
    assert math.copysign(1, v.y) == 1
    assert v.z == 1


def test_vector_zero_and_is_zero():
    v = Vector(1, 2, 3)
    assert not v.is_zero
    assert v.zero() is v
    assert v.is_zero
    assert xyz(v) == (0, 0, 0)
    assert Vector().is_zero
    assert not Vector(0, 0, 0.001).is_zero


def test_vector_rotate_around_axes():
    quarter = math.pi / 2
    assert xyz(Vector(1, 0, 0).rotate_around_x(quarter)) == approx(1, 0, 0)
    assert xyz(Vector(0, 1, 0).rotate_around_x(quarter)) == approx(0, 0, 1)
    assert xyz(Vector(1, 0, 0).rotate_around_y(quarter)) == approx(0, 0, -1)
    assert xyz(Vector(0, 0, 1).rotate_around_y(quarter)) == approx(1, 0, 0)
    assert xyz(Vector(1, 0, 0).rotate_around_z(quarter)) == approx(0, 1, 0)
    assert xyz(Vector(0, 1, 0).rotate_around_z(quarter)) == approx(-1, 0, 0)
    assert xyz(Vector(1, 2, 3).rotate_around_z(math.pi)) == approx(-1, -2, 3)
    assert xyz(Vector(1, 2, 3).rotate_around_y(0)) == approx(1, 2, 3)


def test_vector_rotate_mutates_and_returns_self():
    v = Vector(1, 0, 0)
    assert v.rotate_around_z(math.pi / 2) is v
    assert xyz(v) == approx(0, 1, 0)


def test_vector_rotate_around_axis():
    quarter = math.pi / 2
    assert xyz(Vector(1, 0, 0).rotate_around_axis(Vector(0, 0, 1), quarter)) == approx(0, 1, 0)
    assert xyz(Vector(1, 0, 0).rotate_around_axis(Vector(0, 0, 2), quarter)) == approx(0, 1, 0)
    assert xyz(Vector(1, 0, 0).rotate_around_axis(Vector(0, 0, 1), quarter, False)) == approx(0, 1, 0)
    assert xyz(Vector(1, 0, 0).rotate_around_axis(Vector(0, 1, 0), quarter)) == approx(0, 0, -1)
    assert xyz(Vector(0, 5, 0).rotate_around_axis(Vector(0, 1, 0), 1.234)) == approx(0, 5, 0)
    diagonal = Vector(1, 1, 1)
    assert xyz(Vector(1, 2, 3).rotate_around_axis(diagonal, 2 * math.pi / 3)) == approx(3, 1, 2)
    rotated = Vector(1, 2, 3).rotate_around_axis(Vector(1, 2, 2), 0.7)
    assert rotated.length == pytest.approx(Vector(1, 2, 3).length, abs=1e-5)


def test_vector_is_in_aabb():
    low = Vector(0, 0, 0)
    high = Vector(2, 4, 6)
    assert Vector(1, 2, 3).is_in_aabb(low, high)
    assert Vector(0, 0, 0).is_in_aabb(low, high)
    assert Vector(2, 4, 6).is_in_aabb(low, high)
    assert not Vector(2.1, 2, 3).is_in_aabb(low, high)
    assert not Vector(1, -0.1, 3).is_in_aabb(low, high)
    assert not Vector(1, 2, 6.5).is_in_aabb(low, high)


def test_vector_is_in_sphere():
    origin = Vector(1, 1, 1)
    assert Vector(1, 1, 1).is_in_sphere(origin, 0)
    assert Vector(1, 3, 1).is_in_sphere(origin, 2)
    assert Vector(1, 3, 1).is_in_sphere(origin, 3)
    assert not Vector(1, 3.5, 1).is_in_sphere(origin, 2)
    assert not Vector(4, 5, 1).is_in_sphere(origin, 4.9)
    assert Vector(4, 5, 1).is_in_sphere(origin, 5.1)


def test_location_str_repr_and_eq(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    nether = server.level.get_dimension(Dimension.NETHER)
    loc = Location(overworld, 1, 2, 3, 10, 20)
    assert "x=1" in str(loc) and "y=2" in str(loc) and "z=3" in str(loc)
    assert "pitch=10" in str(loc) and "yaw=20" in str(loc)
    assert loc == Location(overworld, 1, 2, 3, 10, 20)
    assert not loc != Location(overworld, 1, 2, 3, 10, 20)
    assert loc != Location(overworld, 1, 2, 4, 10, 20)
    assert loc != Location(overworld, 1, 2, 3, 11, 20)
    assert loc != Location(overworld, 1, 2, 3, 10, 21)
    assert loc != Location(nether, 1, 2, 3, 10, 20)
    assert not loc == Location(nether, 1, 2, 3, 10, 20)
    assert Location(None, 1, 2, 3) == Location(None, 1, 2, 3)
    assert Location(None, 1, 2, 3) != loc


def test_location_arithmetic(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    loc = Location(overworld, 1, 2, 3, 10, 20)
    other = Location(overworld, 4, 6, 3)

    total = loc + other
    assert (total.x, total.y, total.z, total.pitch, total.yaw) == (5, 8, 6, 10, 20)
    assert total.dimension == overworld
    diff = loc - other
    assert (diff.x, diff.y, diff.z, diff.pitch, diff.yaw) == (-3, -4, 0, 10, 20)
    shifted = loc + Vector(1, 1, 1)
    assert (shifted.x, shifted.y, shifted.z) == (2, 3, 4)
    shifted = loc - Vector(1, 1, 1)
    assert (shifted.x, shifted.y, shifted.z) == (0, 1, 2)
    scaled = loc * 2
    assert (scaled.x, scaled.y, scaled.z, scaled.pitch, scaled.yaw) == (2, 4, 6, 10, 20)
    assert (loc.x, loc.y, loc.z) == (1, 2, 3)


def test_location_in_place_arithmetic(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    loc = Location(overworld, 1, 2, 3, 10, 20)
    alias = loc
    loc += Location(overworld, 1, 1, 1)
    assert loc is alias and (alias.x, alias.y, alias.z) == (2, 3, 4)
    loc -= Location(overworld, 2, 2, 2)
    assert loc is alias and (alias.x, alias.y, alias.z) == (0, 1, 2)
    loc += Vector(1, 1, 1)
    assert loc is alias and (alias.x, alias.y, alias.z) == (1, 2, 3)
    loc -= Vector(1, 2, 3)
    assert loc is alias and (alias.x, alias.y, alias.z) == (0, 0, 0)
    loc += Vector(1, 2, 3)
    loc *= 3
    assert loc is alias and (alias.x, alias.y, alias.z) == (3, 6, 9)
    assert (alias.pitch, alias.yaw) == (10, 20)
    assert alias.dimension == overworld


def _iadd(a, b):
    a += b


def _isub(a, b):
    a -= b


@pytest.mark.parametrize(
    "operation",
    [lambda a, b: a + b, lambda a, b: a - b, _iadd, _isub],
    ids=["add", "sub", "iadd", "isub"],
)
def test_location_arithmetic_across_dimensions_is_rejected(server: Server, operation):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    nether = server.level.get_dimension(Dimension.NETHER)
    a = Location(overworld, 1, 2, 3)
    b = Location(nether, 1, 2, 3)
    with pytest.raises(ValueError):
        operation(a, b)


def test_location_distance(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    nether = server.level.get_dimension(Dimension.NETHER)
    a = Location(overworld, 1, 2, 3, 90, 90)
    b = Location(overworld, 4, 6, 3)
    assert a.distance(b) == pytest.approx(5)
    assert a.distance_squared(b) == pytest.approx(25)
    assert b.distance(a) == pytest.approx(5)
    assert a.distance(a) == 0
    with pytest.raises(ValueError):
        a.distance(Location(nether, 4, 6, 3))
    with pytest.raises(ValueError):
        a.distance_squared(Location(nether, 4, 6, 3))
    with pytest.raises(ValueError):
        Location(None, 0, 0, 0).distance(Location(None, 0, 0, 0))


def test_location_length_ignores_world_and_orientation(server: Server):
    loc = Location(server.level.get_dimension(Dimension.OVERWORLD), 3, 4, 12, 45, 45)
    assert loc.length == pytest.approx(13)
    assert loc.length_squared == pytest.approx(169)
    assert Location(None, 0, 0, 0).length == 0


def test_location_component_setters_move_the_block(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    loc = Location(overworld, 0, 0, 0)
    loc.x = 5.5
    loc.y = 64.25
    loc.z = -3.5
    assert (loc.x, loc.y, loc.z) == (5.5, 64.25, -3.5)
    assert (loc.block_x, loc.block_y, loc.block_z) == (5, 64, -4)
    assert loc.length_squared == pytest.approx(5.5**2 + 64.25**2 + 3.5**2)
    assert loc.distance(Location(overworld, 5.5, 64.25, -3.5)) == 0


def test_location_zero_keeps_dimension_and_rotation(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    loc = Location(overworld, 1, 2, 3, 10, 20)
    assert loc.zero() is loc
    assert (loc.x, loc.y, loc.z) == (0, 0, 0)
    assert (loc.pitch, loc.yaw) == (10, 20)
    assert loc.dimension == overworld
    assert loc.length == 0


@pytest.mark.parametrize(
    "yaw, expected",
    [(0, 0), (190, -170), (-190, 170), (360, 0), (540, -180), (180, -180), (-180, -180), (179, 179), (725, 5)],
)
def test_location_normalize_yaw(yaw, expected):
    assert Location.normalize_yaw(yaw) == pytest.approx(expected)


@pytest.mark.parametrize(
    "pitch, expected", [(0, 0), (45.5, 45.5), (91, 90), (-91, -90), (90, 90), (1e9, 90), (-1e9, -90)]
)
def test_location_normalize_pitch(pitch, expected):
    assert Location.normalize_pitch(pitch) == pytest.approx(expected)


def test_location_direction_get(server: Server):
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    assert xyz(Location(overworld, 0, 0, 0, 0, 0).direction) == approx(0, 0, 1)
    assert xyz(Location(overworld, 0, 0, 0, 0, 90).direction) == approx(-1, 0, 0)
    assert xyz(Location(overworld, 0, 0, 0, 0, 180).direction) == approx(0, 0, -1)
    assert xyz(Location(overworld, 0, 0, 0, 0, 270).direction) == approx(1, 0, 0)
    assert xyz(Location(overworld, 0, 0, 0, 90, 0).direction) == approx(0, -1, 0)
    assert xyz(Location(overworld, 0, 0, 0, -90, 0).direction) == approx(0, 1, 0)
    pitched = Location(overworld, 0, 0, 0, 10, 20).direction
    assert xyz(pitched) == approx(
        -math.cos(math.radians(10)) * math.sin(math.radians(20)),
        -math.sin(math.radians(10)),
        math.cos(math.radians(10)) * math.cos(math.radians(20)),
    )
    assert pitched.length == pytest.approx(1, abs=1e-5)


@pytest.mark.parametrize(
    "direction, pitch, yaw",
    [
        ((0, 0, 1), 0, 0),
        ((1, 0, 0), 0, 270),
        ((0, 0, -1), 0, 180),
        ((-1, 0, 0), 0, 90),
        ((0, 1, 0), -90, None),
        ((0, -1, 0), 90, None),
    ],
)
def test_location_direction_set_axes(server: Server, direction, pitch, yaw):
    loc = Location(server.level.get_dimension(Dimension.OVERWORLD), 7, 8, 9, 0, 33)
    loc.direction = Vector(*direction)
    assert loc.pitch == pytest.approx(pitch)
    assert loc.yaw == pytest.approx(33 if yaw is None else yaw)
    assert (loc.x, loc.y, loc.z) == (7, 8, 9)


def test_location_direction_set_roundtrips(server: Server):
    loc = Location(server.level.get_dimension(Dimension.OVERWORLD), 0, 0, 0)
    wanted = Vector(1, 1, 0)
    wanted.normalize()
    loc.direction = Vector(1, 1, 0)
    assert loc.pitch == pytest.approx(-45, abs=1e-3)
    assert loc.yaw == pytest.approx(270, abs=1e-3)
    assert xyz(loc.direction) == approx(*xyz(wanted))
    loc.direction = Vector(-2, -3, 5)
    expected = Vector(-2, -3, 5).normalize()
    assert xyz(loc.direction) == approx(*xyz(expected))


def test_location_block_reads_the_world(server: Server, plot):
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(2, 0, 3)
    block = overworld.get_block_at(x, y, z)
    original = block.type
    try:
        block.set_type("minecraft:gold_block")
        loc = Location(overworld, x + 0.5, y + 0.25, z + 0.75)
        assert loc.block.type == overworld.get_block_at(x, y, z).type
        assert str(loc.block.type) == "minecraft:gold_block"
        assert (loc.block.x, loc.block.y, loc.block.z) == (x, y, z)
        above = Location(overworld, x + 0.5, y + 1.5, z + 0.5)
        assert str(above.block.type) == "minecraft:air"
        assert (above.block.x, above.block.y, above.block.z) == (x, y + 1, z)
    finally:
        block.set_type(original)


def test_compound_tag_iter_yields_keys():
    compound = CompoundTag({"a": IntTag(1), "b": StringTag("x"), "c": ByteTag(3)})
    iterator = iter(compound)
    assert sorted(iterator) == ["a", "b", "c"]
    assert sorted(compound) == sorted(compound.keys())
    assert list(iter(CompoundTag())) == []
    assert all(isinstance(key, str) for key in compound)
    assert [compound[key] for key in compound] == [compound[key] for key in compound.keys()]
    compound.clear()
    assert list(compound) == []


def test_compound_tag_iter_tracks_edits():
    compound = CompoundTag()
    compound["first"] = IntTag(1)
    assert list(compound) == ["first"]
    compound["second"] = IntTag(2)
    assert sorted(compound) == ["first", "second"]
    compound.pop("first")
    assert list(compound) == ["second"]


def test_list_tag_extend_appends_in_order():
    tags = ListTag([IntTag(1), IntTag(2)])
    tags.extend([IntTag(3), IntTag(4)])
    assert [tag.value for tag in tags] == [1, 2, 3, 4]
    tags.extend(ListTag([IntTag(5)]))
    tags.extend(IntTag(n) for n in (6, 7))
    tags.extend([])
    assert [tag.value for tag in tags] == [1, 2, 3, 4, 5, 6, 7]
    assert len(tags) == 7


def test_list_tag_extend_empty_list_fixes_the_element_type():
    tags = ListTag()
    tags.extend([ByteTag(1), ByteTag(2)])
    assert len(tags) == 2
    with pytest.raises(ValueError):
        tags.extend([StringTag("s")])


def test_list_tag_extend_rejects_mixed_types_and_keeps_the_list_valid():
    tags = ListTag([IntTag(1)])
    with pytest.raises(ValueError):
        tags.extend([StringTag("s")])
    assert [tag.value for tag in tags] == [1]
    tags.append(IntTag(2))
    assert len(tags) == 2


def test_list_tag_clear_empties_and_resets_the_element_type():
    tags = ListTag([IntTag(1), IntTag(2)])
    tags.clear()
    assert len(tags) == 0
    assert list(tags) == []
    assert tags == ListTag()
    tags.append(StringTag("now strings"))
    assert [tag.value for tag in tags] == ["now strings"]
    tags.clear()
    tags.clear()
    assert len(tags) == 0
