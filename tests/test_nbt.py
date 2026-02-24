import pytest

from endstone.nbt import (
    ByteArrayTag,
    ByteTag,
    CompoundTag,
    DoubleTag,
    FloatTag,
    IntArrayTag,
    IntTag,
    ListTag,
    LongTag,
    ShortTag,
    StringTag,
    Tag,
)


@pytest.mark.parametrize(
    "cls, py_type",
    [
        (ByteTag, int),
        (ShortTag, int),
        (IntTag, int),
        (LongTag, int),
    ],
)
def test_integer_tags_basic(cls, py_type):
    # default constructor
    t0 = cls()
    assert isinstance(t0.value, py_type)
    # assume default is zero-ish
    assert t0 == 0
    assert int(t0) == 0

    # value constructor
    t1 = cls(42)
    assert isinstance(t1.value, py_type)
    assert t1.value == 42
    assert int(t1) == 42

    # equality between tags
    t2 = cls(42)
    t3 = cls(7)
    assert t1 == t2
    assert not (t1 != t2)
    assert t1 != t3
    assert not (t1 == t3)

    # equality with python scalar
    assert t1 == 42
    assert t1 != 7


@pytest.mark.parametrize("cls", [FloatTag, DoubleTag])
def test_float_tags_basic(cls):
    # default constructor
    t0 = cls()
    assert isinstance(t0.value, float)
    # default value is 0.0
    assert t0 == 0.0
    assert float(t0) == pytest.approx(0.0)

    # value constructor
    t1 = cls(1.5)
    assert isinstance(t1.value, float)
    assert t1.value == pytest.approx(1.5)
    assert float(t1) == pytest.approx(1.5)

    # equality between tags
    t2 = cls(1.5)
    t3 = cls(2.0)
    assert t1 == t2
    assert not (t1 != t2)
    assert t1 != t3
    assert not (t1 == t3)

    # equality with python float
    assert t1 == 1.5
    assert t1 != 2.0


def test_string_tag_basic():
    s0 = StringTag()
    assert isinstance(s0.value, str)
    # default empty string
    assert s0.value == ""
    assert s0 == ""

    s1 = StringTag("hello")
    assert s1.value == "hello"
    assert s1 == "hello"
    assert s1 != "world"

    s2 = StringTag("hello")
    assert s1 == s2
    assert not (s1 != s2)

    # str/repr should be strings (do not overspec format)
    assert isinstance(str(s1), str)
    assert isinstance(repr(s1), str)


def test_byte_array_tag_empty_and_truthiness():
    ba = ByteArrayTag()
    assert len(ba) == 0
    assert not ba  # empty is falsy

    ba.append(1)
    assert len(ba) == 1
    assert bool(ba)


def test_byte_array_tag_append_extend_and_indexing():
    ba = ByteArrayTag()
    ba.append(1)
    ba.append(2)
    assert len(ba) == 2
    assert list(ba) == [1, 2]

    # __getitem__
    assert ba[0] == 1
    assert ba[1] == 2

    # __setitem__
    ba[0] = 5
    assert ba[0] == 5
    assert list(ba) == [5, 2]

    # extend
    ba.extend([7, 8])
    assert list(ba) == [5, 2, 7, 8]

    # iteration type
    for v in ba:
        assert isinstance(v, int)


def test_byte_array_tag_clear_and_equality():
    ba1 = ByteArrayTag([1, 2, 3])
    ba2 = ByteArrayTag([1, 2, 3])
    ba3 = ByteArrayTag([1, 2])

    assert ba1 == ba2
    assert not (ba1 != ba2)
    assert ba1 != ba3
    assert not (ba1 == ba3)

    # equality with python list
    assert ba1 == [1, 2, 3]
    assert ba1 != [1, 2]

    # clear
    ba1.clear()
    assert len(ba1) == 0
    assert list(ba1) == []


def test_byte_array_tag_bytes_roundtrip():
    data = [1, 2, 3, 255]
    ba = ByteArrayTag(data)
    b = bytes(ba)
    # ensure length matches
    assert len(b) == len(data)
    # interpret as unsigned bytes
    assert list(b) == [x & 0xFF for x in data]


def test_int_array_tag_empty_and_truthiness():
    ia = IntArrayTag()
    assert len(ia) == 0
    assert not ia

    ia.append(10)
    assert len(ia) == 1
    assert bool(ia)


def test_int_array_tag_append_extend_and_indexing():
    ia = IntArrayTag()
    ia.append(1)
    ia.append(2)
    assert len(ia) == 2
    assert list(ia) == [1, 2]

    assert ia[0] == 1
    assert ia[1] == 2

    ia[0] = 5
    assert ia[0] == 5
    assert list(ia) == [5, 2]

    ia.extend([7, 8])
    assert list(ia) == [5, 2, 7, 8]

    for v in ia:
        assert isinstance(v, int)


def test_int_array_tag_clear_and_equality():
    ia1 = IntArrayTag([1, 2, 3])
    ia2 = IntArrayTag([1, 2, 3])
    ia3 = IntArrayTag([1, 2])

    assert ia1 == ia2
    assert not (ia1 != ia2)
    assert ia1 != ia3
    assert not (ia1 == ia3)

    assert ia1 == [1, 2, 3]
    assert ia1 != [1, 2]

    ia1.clear()
    assert len(ia1) == 0
    assert list(ia1) == []


def test_list_tag_construct_from_list_iterable():
    items = [
        IntTag(1),
        IntTag(2),
        IntTag(3),
    ]
    lt = ListTag(items)

    assert len(lt) == 3
    assert lt.size() == 3
    assert not lt.empty()

    # elements should match and stay homogeneous
    for idx, tag in enumerate(lt):
        assert isinstance(tag, Tag)
        assert isinstance(tag, IntTag)
        assert tag == items[idx]

    # equality with another ListTag built via append
    lt2 = ListTag()
    for item in items:
        lt2.append(item)
    assert lt == lt2
    assert not (lt != lt2)


def test_list_tag_construct_from_generator_iterable():
    def gen():
        for i in range(3):
            yield IntTag(i)

    lt = ListTag(gen())

    assert len(lt) == 3
    assert [t.value for t in lt] == [0, 1, 2]

    # still homogeneous
    for tag in lt:
        assert isinstance(tag, IntTag)


def test_list_tag_empty_and_truthiness():
    lt = ListTag()
    assert len(lt) == 0
    assert lt.size() == 0
    assert lt.empty()
    assert not lt

    lt.append(IntTag(1))
    assert len(lt) == 1
    assert lt.size() == 1
    assert not lt.empty()
    assert bool(lt)


def test_list_tag_indexing_and_modification():
    lt = ListTag()
    t1 = IntTag(1)
    t2 = IntTag(2)
    lt.append(t1)
    lt.append(t2)

    assert lt[0] == t1
    assert lt[1] == t2

    # __setitem__
    t3 = IntTag(3)
    lt[1] = t3
    assert lt[1] == t3

    # __delitem__
    del lt[0]
    assert len(lt) == 1
    assert lt[0] == t3

    # pop
    popped = lt.pop()
    assert isinstance(popped, Tag)
    assert len(lt) == 0

    # bounds check
    with pytest.raises(IndexError):
        _ = lt[0]


def test_list_tag_iteration_and_equality():
    lt1 = ListTag()
    lt2 = ListTag()
    lt3 = ListTag()

    # all IntTag — homogeneous
    lt1.append(IntTag(1))
    lt1.append(IntTag(2))

    lt2.append(IntTag(1))
    lt2.append(IntTag(2))

    lt3.append(IntTag(1))
    lt3.append(IntTag(3))  # same type, different value

    # iteration yields Tag instances
    for item in lt1:
        assert isinstance(item, Tag)

    assert lt1 == lt2
    assert not (lt1 != lt2)

    assert lt1 != lt3
    assert not (lt1 == lt3)


def test_list_tag_rejects_inhomogeneous_items():
    lt = ListTag()
    lt.append(IntTag(1))

    with pytest.raises(ValueError):
        lt.append(StringTag("no"))  # wrong element type


def test_list_tag_constructor_homogeneity():
    with pytest.raises(ValueError):
        ListTag([IntTag(1), StringTag("x")])


def test_compound_tag_construct_from_mapping_subclass():
    mapping = dict(
        health=IntTag(10),
        armor=IntTag(5),
    )

    ct = CompoundTag(mapping)

    assert len(ct) == 2
    assert "health" in ct and "armor" in ct
    assert ct["health"] == 10
    assert ct["armor"] == 5

    # round-trip through dict(ct.items())
    roundtrip = dict(ct.items())
    assert isinstance(roundtrip["health"], IntTag)
    assert isinstance(roundtrip["armor"], IntTag)


def test_compound_tag_empty_and_truthiness():
    ct = CompoundTag()
    assert len(ct) == 0
    assert not ct

    ct["x"] = IntTag(1)
    assert len(ct) == 1
    assert bool(ct)


def test_compound_tag_item_access_and_membership():
    ct = CompoundTag()
    t = IntTag(42)
    ct["answer"] = t

    assert "answer" in ct
    assert "missing" not in ct

    value = ct["answer"]
    assert value == t
    assert isinstance(value, IntTag)

    # keys / values / items
    keys = list(ct.keys())
    values = list(ct.values())
    items = list(ct.items())

    assert keys == ["answer"]
    assert values[0] == t
    assert items[0] == ("answer", t)

    # __contains__ should work with key
    assert "answer" in ct


def test_compound_tag_setdefault_and_pop():
    ct = CompoundTag()

    # setdefault inserts if missing
    v1 = ct.setdefault("k1", IntTag(1))
    assert isinstance(v1, IntTag)
    assert v1 == 1
    assert "k1" in ct

    # setdefault does not overwrite existing
    v2 = ct.setdefault("k1", IntTag(2))
    assert v2 == 1
    assert ct["k1"] == 1

    # pop existing
    popped = ct.pop("k1")
    assert isinstance(popped, IntTag)
    assert "k1" not in ct

    # pop missing with default
    sentinel = IntTag()
    res = ct.pop("nope", sentinel)
    assert res is sentinel

    # pop missing without default should raise
    with pytest.raises(KeyError):
        ct.pop("nope")


def test_compound_tag_clear_and_equality():
    ct1 = CompoundTag()
    ct1["a"] = IntTag(1)
    ct1["b"] = StringTag("x")

    ct2 = CompoundTag({"a": IntTag(1), "b": StringTag("x")})
    ct3 = CompoundTag({"a": IntTag(2), "b": StringTag("x")})

    assert ct1 == ct2
    assert not (ct1 != ct2)
    assert ct1 != ct3
    assert not (ct1 == ct3)

    ct1.clear()
    assert len(ct1) == 0
    assert list(ct1.keys()) == []


def test_nested_compound_tags():
    c1 = CompoundTag(
        {
            "pos": CompoundTag(
                {
                    "x": IntTag(1),
                    "y": IntTag(2),
                }
            ),
            "name": StringTag("Player"),
        }
    )

    c2 = CompoundTag()
    c2["pos"] = CompoundTag()
    c2["pos"]["x"] = IntTag(1)
    c2["pos"]["y"] = IntTag(2)
    c2["name"] = StringTag("Player")

    # equality should be deep
    assert c1 == c2
    assert not (c1 != c2)

    # mutate nested inner compound; equality should break
    c2["pos"]["y"] = IntTag(3)
    assert c1 != c2
    assert not (c1 == c2)


@pytest.mark.parametrize(
    "tag,expected",
    [
        (ByteTag(1), "1b"),
        (ShortTag(2), "2s"),
        (IntTag(3), "3"),
        (LongTag(4), "4L"),
        (FloatTag(1.0), "1.0f"),
        (DoubleTag(2.0), "2.0d"),
        (ByteArrayTag([1]), "[B;1b]"),
        (ByteArrayTag([1, 2, 3]), "[B;1b,2b,3b]"),
        (StringTag("hello"), "hello"),
        (StringTag('Call me "Ishmael"'), "'Call me \"Ishmael\"'"),
        (StringTag("Call me 'Ishmael'"), "\"Call me 'Ishmael'\""),
        (IntArrayTag([1]), "[I;1]"),
        (IntArrayTag([1, 2, 3]), "[I;1,2,3]"),
        (ListTag([IntTag(1), IntTag(2)]), "[1,2]"),
        (
            ListTag(
                [
                    CompoundTag({"id": IntTag(1)}),
                    CompoundTag({"id": IntTag(2)}),
                ]
            ),
            "[{id:1},{id:2}]",
        ),
        (
            CompoundTag({"x": IntTag(1), "y": StringTag("a")}),
            "{x:1,y:a}",
        ),
        (
            CompoundTag(
                {
                    "pos": ListTag([IntTag(1), IntTag(2), IntTag(3)]),
                    "name": StringTag("Alex"),
                }
            ),
            "{name:Alex,pos:[1,2,3]}",
        ),
    ],
)
def test_str_tags(tag, expected):
    assert str(tag) == expected


@pytest.mark.parametrize(
    "tag,expected",
    [
        (ByteTag(1), "ByteTag(1)"),
        (ShortTag(2), "ShortTag(2)"),
        (IntTag(3), "IntTag(3)"),
        (LongTag(4), "LongTag(4)"),
        (FloatTag(1.0), "FloatTag(1.0)"),
        (DoubleTag(2.0), "DoubleTag(2.0)"),
        (StringTag("hello"), "StringTag('hello')"),
        (ByteArrayTag([1, 2, 3]), "ByteArrayTag([1, 2, 3])"),
        (IntArrayTag([1, 2, 3]), "IntArrayTag([1, 2, 3])"),
        (ListTag([IntTag(1), IntTag(2)]), "ListTag([IntTag(1), IntTag(2)])"),
        (
            CompoundTag({"x": IntTag(1)}),
            "CompoundTag({'x': IntTag(1)})",
        ),
    ],
)
def test_repr_tags(tag, expected):
    assert repr(tag) == expected


def test_to_list_empty():
    lt = ListTag()
    assert lt.to_list() == []


def test_to_list_scalars():
    lt = ListTag([IntTag(1), IntTag(2), IntTag(3)])
    result = lt.to_list()
    assert result == [1, 2, 3]
    assert all(isinstance(v, int) for v in result)


def test_to_list_strings():
    lt = ListTag([StringTag("a"), StringTag("b")])
    result = lt.to_list()
    assert result == ["a", "b"]
    assert all(isinstance(v, str) for v in result)


def test_to_list_nested_compounds():
    lt = ListTag(
        [
            CompoundTag({"x": IntTag(1)}),
            CompoundTag({"x": IntTag(2)}),
        ]
    )
    result = lt.to_list()
    assert result == [{"x": 1}, {"x": 2}]
    assert all(isinstance(v, dict) for v in result)


def test_to_list_does_not_mutate_original():
    lt = ListTag([IntTag(10), IntTag(20)])
    result = lt.to_list()
    result[0] = 999
    result.append(30)
    assert len(lt) == 2
    assert lt[0] == IntTag(10)


def test_to_dict_empty():
    ct = CompoundTag()
    assert ct.to_dict() == {}


def test_to_dict_scalar_types():
    ct = CompoundTag(
        {
            "byte": ByteTag(1),
            "short": ShortTag(2),
            "int": IntTag(3),
            "long": LongTag(4),
            "float": FloatTag(1.5),
            "double": DoubleTag(2.5),
            "string": StringTag("hello"),
        }
    )
    d = ct.to_dict()
    assert d == {
        "byte": 1,
        "short": 2,
        "int": 3,
        "long": 4,
        "float": pytest.approx(1.5),
        "double": pytest.approx(2.5),
        "string": "hello",
    }
    # verify native Python types
    assert isinstance(d["byte"], int)
    assert isinstance(d["short"], int)
    assert isinstance(d["int"], int)
    assert isinstance(d["long"], int)
    assert isinstance(d["float"], float)
    assert isinstance(d["double"], float)
    assert isinstance(d["string"], str)


def test_to_dict_byte_array_returns_bytes():
    ct = CompoundTag({"data": ByteArrayTag([1, 2, 255])})
    d = ct.to_dict()
    assert isinstance(d["data"], bytes)
    assert d["data"] == bytes([1, 2, 255])


def test_to_dict_int_array_returns_list():
    ct = CompoundTag({"ids": IntArrayTag([10, 20, 30])})
    d = ct.to_dict()
    assert isinstance(d["ids"], list)
    assert d["ids"] == [10, 20, 30]


def test_to_dict_list_tag():
    ct = CompoundTag({"scores": ListTag([IntTag(100), IntTag(200)])})
    d = ct.to_dict()
    assert isinstance(d["scores"], list)
    assert d["scores"] == [100, 200]


def test_to_dict_nested_compound():
    ct = CompoundTag(
        {
            "pos": CompoundTag(
                {
                    "x": IntTag(1),
                    "y": IntTag(2),
                    "z": IntTag(3),
                }
            ),
            "name": StringTag("Player"),
        }
    )
    d = ct.to_dict()
    assert isinstance(d["pos"], dict)
    assert d["pos"] == {"x": 1, "y": 2, "z": 3}
    assert d["name"] == "Player"


def test_to_dict_nested_list_of_compounds():
    ct = CompoundTag(
        {
            "items": ListTag(
                [
                    CompoundTag({"id": StringTag("stone"), "count": ByteTag(64)}),
                    CompoundTag({"id": StringTag("dirt"), "count": ByteTag(32)}),
                ]
            ),
        }
    )
    d = ct.to_dict()
    assert isinstance(d["items"], list)
    assert len(d["items"]) == 2
    assert d["items"][0] == {"id": "stone", "count": 64}
    assert d["items"][1] == {"id": "dirt", "count": 32}


def test_to_dict_does_not_mutate_original():
    ct = CompoundTag({"a": IntTag(1), "b": StringTag("x")})
    d = ct.to_dict()
    d["a"] = 999
    d["c"] = "new"
    # original should be unchanged
    assert ct["a"] == 1
    assert "c" not in ct


def test_compound_tag_self_assignment():
    a = CompoundTag({"a": IntTag(1)})
    a["b"] = a

    # a now has two keys
    assert len(a) == 2
    assert "a" in a and "b" in a
    assert a["a"] == 1

    # a["b"] is a CompoundTag (deep copy snapshot taken BEFORE insertion)
    assert isinstance(a["b"], CompoundTag)
    inner = a["b"]
    assert len(inner) == 1
    assert inner["a"] == 1
