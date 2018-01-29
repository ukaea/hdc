from cyhdc import HDC
import numpy as np

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

    print(tree.dumps())
    print(tree['sub/sub'].dumps())

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

    print("-- set data 2D")
    x2d = np.arange(12, dtype=np.float64).reshape((2, -1))
    print(x2d)
    h = HDC()
    h.set_data(x2d)
    print(h.dumps())
    x2d2 = np.asarray(h)
    print(x2d2)

    print("-- set data 3D")
    x3d = np.arange(24, dtype=np.float64).reshape((4, 3, -1))
    print(x3d)
    h = HDC()
    h.set_data(x3d)
    print(h.dumps())
    x3d2 = np.asarray(h)
    print(x3d2)
    assert x3d2.shape == x3d.shape
    assert x3d2.size == x3d.size
    assert x3d2.dtype == x3d.dtype
    assert x3d2.strides == x3d.strides
    assert np.all(x3d2 == x3d)


    print("-- set data 2D float32")
    x2d = np.arange(12, dtype=np.float32).reshape((2, -1))
    print(x2d)
    h = HDC()
    h.set_data(x2d)
    print(h.dumps())
    x2d2 = np.asarray(h)
    print(x2d2)
    assert x2d.shape == x2d2.shape
    assert x2d.size == x2d2.size
    assert x2d.dtype == x2d2.dtype
    assert x2d.strides == x2d2.strides
    assert np.all(x2d == x2d2)

    print("-- set data 2D int32")
    x2d = np.arange(12, dtype=np.int32).reshape((2, -1))
    print(x2d)
    h = HDC()
    h.set_data(x2d)
    print(h.dumps())
    x2d2 = np.asarray(h)
    print(x2d2)
    assert x2d.shape == x2d2.shape
    assert x2d.size == x2d2.size
    assert x2d.dtype == x2d2.dtype
    assert x2d.strides == x2d2.strides
    assert np.all(x2d == x2d2)

    print("-- set data 2D int64")
    x2d = np.arange(12, dtype=np.int64).reshape((2, -1))
    print(x2d)
    h = HDC()
    h.set_data(x2d)
    print(h.dumps())
    x2d2 = np.asarray(h)
    print(x2d2)
    assert x2d.shape == x2d2.shape
    assert x2d.size == x2d2.size
    assert x2d.dtype == x2d2.dtype
    assert x2d.strides == x2d2.strides
    assert np.all(x2d == x2d2)

    print("-- set data 2D int16")
    x2d = np.arange(12, dtype=np.int16).reshape((2, -1))
    print(x2d)
    h = HDC()
    h.set_data(x2d)
    print(h.dumps())
    x2d2 = np.asarray(h)
    print(x2d2)
    assert x2d.shape == x2d2.shape
    assert x2d.size == x2d2.size
    assert x2d.dtype == x2d2.dtype
    assert x2d.strides == x2d2.strides
    assert np.all(x2d == x2d2)

    print("-- set data 2D float16")
    x2d = np.arange(12, dtype=np.float16).reshape((2, -1))
    print(x2d)
    h = HDC()
    h.set_data(x2d)
    print(h.dumps())
    x2d2 = np.asarray(h)
    print(x2d2)
    assert x2d.shape == x2d2.shape
    assert x2d.size == x2d2.size
    assert x2d.dtype == x2d2.dtype
    assert x2d.strides == x2d2.strides
    assert np.all(x2d == x2d2)


if __name__ == '__main__':
    test_1()
