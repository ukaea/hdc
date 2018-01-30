import pytest
from cyhdc import HDC
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


def test_list_type():
    h = HDC()
    test_list = ["jedna", "dva"]
    h.append(test_list[0])
    h.append(test_list[1])
    assert test_list == json.loads(h.dumps())

    test_list = ["jedna", "dva"]
    h = HDC(test_list)
    assert test_list == json.loads(h.dumps())


def test_map_type():
    h = HDC()
    test_map = {"jedna": "one", "dva": "two"}
    h = HDC(test_map)
    assert test_map == json.loads(h.dumps())


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


if __name__ == '__main__':
    pytest.main()
