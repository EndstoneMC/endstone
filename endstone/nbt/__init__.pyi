"""
Classes relating to the NBT data format.
"""

import enum

__all__ = ["Tag"]

class Tag:
    class Type(enum.IntEnum):
        END = 0
        BYTE = 1
        SHORT = 2
        INT = 3
        LONG = 4
        FLOAT = 5
        DOUBLE = 6
        BYTE_ARRAY = 7
        STRING = 8
        LIST = 9
        COMPOUND = 10
        INT_ARRAY = 11

    END = Type.END
    BYTE = Type.BYTE
    SHORT = Type.SHORT
    INT = Type.INT
    LONG = Type.LONG
    FLOAT = Type.FLOAT
    DOUBLE = Type.DOUBLE
    BYTE_ARRAY = Type.BYTE_ARRAY
    STRING = Type.STRING
    LIST = Type.LIST
    COMPOUND = Type.COMPOUND
    INT_ARRAY = Type.INT_ARRAY
