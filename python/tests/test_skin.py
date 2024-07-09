import importlib

import pytest
import numpy as np


@pytest.fixture
def cls():
    module = importlib.import_module("endstone")
    return getattr(module, "Skin")


def test_skin_initialization(cls):
    skin_id = "test_skin"
    skin_data = np.array([[[1, 2, 3, 4]], [[5, 6, 7, 8]]], dtype=np.uint8)
    skin = cls(skin_id, skin_data)

    assert skin.skin_id == skin_id
    assert np.array_equal(skin.skin_data, skin_data)


def test_skin_invalid_data_shape(cls):
    skin_id = "test_skin"
    skin_data = np.array([[1, 2, 3, 4], [5, 6, 7, 8]], dtype=np.uint8)

    with pytest.raises(RuntimeError):
        cls(skin_id, skin_data)


def test_skin_with_cape(cls):
    skin_id = "test_skin"
    skin_data = np.array([[[1, 2, 3, 4]], [[5, 6, 7, 8]]], dtype=np.uint8)
    cape_id = "test_cape"
    cape_data = np.array([[[9, 10, 11, 12]], [[13, 14, 15, 16]]], dtype=np.uint8)

    skin = cls(skin_id, skin_data, cape_id, cape_data)

    assert skin.skin_id == skin_id
    assert np.array_equal(skin.skin_data, skin_data)
    assert skin.cape_id == cape_id
    assert np.array_equal(skin.cape_data, cape_data)


def test_skin_without_cape(cls):
    skin_id = "test_skin"
    skin_data = np.array([[[1, 2, 3, 4]], [[5, 6, 7, 8]]], dtype=np.uint8)

    skin = cls(skin_id, skin_data)

    assert skin.skin_id == skin_id
    assert np.array_equal(skin.skin_data, skin_data)
    assert not skin.cape_id
    assert not skin.cape_data
