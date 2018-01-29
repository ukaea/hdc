import pytest
from cyhdc import HDC
import numpy as np


@pytest.mark.parametrize("dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8])
def test_types_1d(dtype):
    x_in = np.arange(5, dtype=dtype)
    h = HDC()
    h.set_data(x_in)
    x_out = np.asarray(h)
    assert x_in.shape == x_out.shape
    assert x_in.size == x_out.size
    assert x_in.dtype == x_out.dtype
    assert x_in.strides == x_out.strides
    assert np.all(x_in == x_out)

@pytest.mark.parametrize("dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8])
def test_types_2d(dtype):
    x_in = np.arange(6, dtype=dtype).reshape((2, -1))
    h = HDC()
    h.set_data(x_in)
    x_out = np.asarray(h)
    assert x_in.shape == x_out.shape
    assert x_in.size == x_out.size
    assert x_in.dtype == x_out.dtype
    assert x_in.strides == x_out.strides
    assert np.all(x_in == x_out)


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


def test_2():

    print("-- set data")
    x = np.arange(30, dtype=np.float64)
    h = HDC()
    h.set_data(x)
    print(h.dumps())

    print("-- set data slice")
    x2 = np.arange(2 * x.size, dtype=np.float64)
    h = HDC()
    h.set_data(x2[::2])
    print(h.dumps())
    print(h.dumps())

    print("-- asarray")
    y = np.asarray(h)
    print(y)
    print('-- modify x2[0] y[1] and dump HDC')
    x2[0] = - 1 / 2
    y[1] = - 1 / 4
    print(h.dumps())


if __name__ == '__main__':
    pytest.main()
