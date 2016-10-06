import numpy as np
from pyhdc import HDC
import random
import string
import itertools
import pytest


def random_array(low=0, high=1e10, ndim=1, maxsize=20, dtype=np.float_):
    # random size and data
    size = np.random.random_integers(1, maxsize, ndim)
    a, b = np.random.random_integers(low, high, size=(2, ))
    data = ((high - low) * np.random.random(size=size) + low).astype(dtype)
    return data


def random_key(nchars=5):
    key = "".join((random.choice(string.ascii_letters) for _ in range(nchars)))
    return key


def test_set_base_types():
    # string
    for size in (0, 1, 10, 101, 999, 100038):
        tree = HDC()
        data = random_key(size)
        tree.set_data(data)
        assert tree.get_data() == data


def test_put_ndarray():
    # loop over number of dimensions
    for ndim in (1, ):
        # loop over types
        for dtype in (np.float_, np.int32, ):
            # create a new container
            tree = HDC()
            # random data
            pydata = random_array()
            # store data into the container with a random key
            key = random_key()
            tree[key] = pydata
            # copy numpy data
            copydata = pydata.copy()
            # and delete the original numpy
            del pydata
            # get the data back from the container
            data = tree[key].asarray()
            # compare the data
            assert np.all(copydata == data)
            assert copydata.dtype == data.dtype


def test_asarray():
    tree = HDC()
    data = random_array()
    tree['data'] = data
    assert np.all(np.asarray(tree['data'] == data))
    assert np.all(tree['data'].asarray() == data)


def test_in_op():
    tree = HDC()
    keys1 = [random_key() for _ in range(3)]
    keys2 = [random_key() for _ in range(3)]
    # put random data
    for key1 in keys1:
        tree[key1] = HDC()
    for key in itertools.chain(keys1, keys2):
        if key in keys1:
            assert key in tree
        else:
            assert key not in tree


def test_nested():
    tree = HDC()
    tree_dict = {}
    keys1 = [random_key() for _ in range(3)]
    keys2 = [random_key() for _ in range(4)]
    # put random data
    for key1 in keys1:
        print(key1)
        tree_dict[key1] = {}
        tree[key1] = HDC()
        for key2 in keys2:
            print(key2)
            data = random_array()
            tree_dict[key1][key2] = data.copy()
            tree[key1][key2] = data
            del data
    # compare data
    for key1 in keys1:
        for key2 in keys2:
            assert np.all(tree_dict[key1][key2] == tree[key1][key2].asarray())

