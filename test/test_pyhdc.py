from pyhdc import HDC
import random
import string
import itertools
import pytest
import numpy as np
import json


@pytest.mark.parametrize("dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8])
@pytest.mark.parametrize("shape", [(), (1, ), (5, ), (1, 1), (1, 3), (4, 1), (6, 8),
                                   (1, 3, 4), (7, 2, 3)])
def test_ndarray(dtype, shape):
    """Create np.array and put/get to/from flat HDC container
    """
    x_in = np.arange(np.prod(shape), dtype=dtype).reshape(shape)
    h = HDC()
    h.set_data(x_in)
    x_out = np.asarray(h)
    assert x_in.shape == x_out.shape
    assert x_in.size == x_out.size
    assert x_in.dtype == x_out.dtype
    assert x_in.strides == x_out.strides
    assert np.all(x_in == x_out)


@pytest.mark.parametrize("dtype", [np.float32, np.float64, np.int32, np.int64])
@pytest.mark.parametrize("shape", [(), (5, ), (6, 8), (7, 2, 3)])
def test_zerocopy(dtype, shape):
    """Test whether asarray is zero copy
    """
    x_in = np.arange(np.prod(shape), dtype=dtype).reshape(shape)
    h = HDC()
    h.set_data(x_in)
    x_out = np.asarray(h)
    x_out *= -1
    x_out_2 = np.asarray(h)
    assert np.all(x_in * -1 == x_out)
    assert np.all(x_out_2 == x_out)
    assert h.shape == shape


def test_list_type():
    h = HDC()
    test_list = ["jedna", "dva"]
    h.append(test_list[0])
    h.append(test_list[1])
    assert test_list == json.loads(h.dumps())

    test_list = ["jedna", "dva"]
    h = HDC(test_list)
    assert test_list == json.loads(h.dumps())
    assert h.shape == (len(test_list), )


def test_map_type():
    h = HDC()
    test_map = {"jedna": "one", "dva": "two"}
    h = HDC(test_map)
    assert test_map == json.loads(h.dumps())
    assert h.shape == (len(test_map), )


def test_str_type():
    h = HDC()
    value = "test string"
    h = HDC(value)
    assert value == str(h)
    assert h.shape == (len(value), )


def test_keys():
    tree = HDC()
    keys = ['a', 'b', 'cc', 'D']
    for key in keys:
        tree[key] = 'value'
    res = list(tree.keys())
    assert res == keys


def test_1():

    empty = HDC()
    assert empty.dumps().strip() == 'null'

    s = HDC('Ahoj')
    print(s.dumps())

    tree = HDC()
    tree['a'] = 'a'
    tree['sub/sub/x'] = 'x'
    assert 'a' in tree
    assert 'sub' in tree
    assert 'A' not in tree
    assert 'SUB' not in tree
    # assert tree['sub']['sub'] == tree['sub/sub']


def random_array(low=0, high=1e10, ndim=1, maxsize=20, dtype=np.float_):
    # random size and data
    size = np.random.random_integers(1, maxsize, ndim)
    a, b = np.random.random_integers(low, high, size=(2, ))
    data = ((high - low) * np.random.random(size=size) + low).astype(dtype)
    return data


def random_key(nchars=5):
    key = "".join((random.choice(string.ascii_letters) for _ in range(nchars)))
    return key


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


def test_in_op_nested():
    tree = HDC()
    keys1 = [random_key() for _ in range(3)]
    keys2 = [random_key() for _ in range(4)]

    # create nested tree
    for key1 in keys1:
        tree[key1] = HDC()
        for key2 in keys2:
            tree[key1][key2] = None

    for key1 in keys1:
        assert key1 in tree
        for key2 in keys2:
            assert key2 in tree[key1]


if __name__ == '__main__':
    pytest.main()
