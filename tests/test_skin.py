import importlib

import numpy as np
import pytest


@pytest.fixture
def cls():
    module = importlib.import_module("endstone")
    return getattr(module, "Skin")


def test_skin_invalid_data_shape(cls):
    skin_id = "test_skin"
    skin_image = np.random.randint(255, size=(16, 32, 2), dtype=np.uint8)

    with pytest.raises(TypeError):
        cls(skin_id, skin_image)


def test_skin_with_cape(cls):
    skin_id = "test_skin"
    skin_image = np.random.randint(255, size=(16, 32, 4), dtype=np.uint8)
    cape_id = "test_cape"
    cape_image = np.random.randint(255, size=(32, 64, 4), dtype=np.uint8)

    skin = cls(skin_id, skin_image, cape_id, cape_image)

    assert skin.id == skin_id
    assert np.array_equal(skin.image, skin_image)
    assert skin.cape_id == cape_id
    assert np.array_equal(skin.cape_image, cape_image)


def test_skin_without_cape(cls):
    skin_id = "test_skin"
    skin_image = np.random.randint(255, size=(16, 32, 4), dtype=np.uint8)
    skin = cls(skin_id, skin_image)

    assert skin.id == skin_id
    assert np.array_equal(skin.image, skin_image)
    assert not skin.cape_id
    assert not skin.cape_image
