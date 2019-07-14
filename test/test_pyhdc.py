import collections
import itertools
import json
import random
import string
import tempfile

from pyhdc import HDC
import numpy as np
import pytest

import future.builtins


@pytest.fixture
def test_trees():
    pytree = {
        "root": {
            "scalars": {"integer": 1, "float": 1.0},
            "arrays": {
                "integer": np.arange(6, dtype=np.int32).reshape((2, -1)),
                "float": np.arange(6, dtype=np.float32).reshape((2, -1)),
            },
            "text": "\tspam & ham\n",
            "list": ["zero", 1, 2.0, "three"],
        }
    }
    hdctree = HDC()
    hdctree["root/scalars/integer"] = HDC(1)
    hdctree["root/scalars/float"] = HDC(1.0)
    hdctree["root/arrays/integer"] = HDC(np.arange(6, dtype=np.int32).reshape((2, -1)))
    hdctree["root/arrays/float"] = HDC(np.arange(6, dtype=np.float32).reshape((2, -1)))
    hdctree["root/text"] = HDC("\tspam & ham\n")
    hdctree["root/list"] = HDC(["zero", 1, 2.0, "three"])

    return pytree, hdctree


def tree_equal(py_obj, hdc_obj, exception=False):
    if isinstance(py_obj, collections.Mapping):
        try:
            res = (len(py_obj) == len(hdc_obj)) and all(
                (
                    tree_equal(py_obj[k1], hdc_obj[k1], exception=exception)
                    if k1 in hdc_obj
                    else False
                    for k1 in py_obj
                )
            )
        except KeyError:
            res = False
        if exception and not res:
            raise ValueError("{} != {}".format(py_obj, hdc_obj))
    elif isinstance(py_obj, future.builtins.str) or isinstance(py_obj, str):
        res = py_obj == future.builtins.str(hdc_obj)
        if exception and not res:
            raise ValueError("{} != {}".format(py_obj, future.builtins.str(hdc_obj)))
    elif isinstance(py_obj, collections.Sequence):
        res = len(py_obj) == len(hdc_obj) and all(
            (tree_equal(v1, v2, exception=exception) for v1, v2 in zip(py_obj, hdc_obj))
        )
        if exception and not res:
            raise ValueError("{} != {}".format(py_obj, hdc_obj))
    else:
        res = np.all(np.asanyarray(py_obj) == np.asanyarray(hdc_obj))
        if exception and not res:
            raise ValueError("{} != {}".format(py_obj, hdc_obj))
    return res


@pytest.mark.parametrize("external", [True, False])
@pytest.mark.parametrize(
    "dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8, np.bool_]
)
@pytest.mark.parametrize(
    "shape",
    [
        (),
        (1,),
        (5,),
        (1, 1),
        (1, 3),
        (4, 1),
        (6, 8),
        (1, 3, 4),
        (7, 2, 3),
        (1, 2, 3, 4),
        (7, 2, 3, 4),
        (1, 2, 3, 4, 5),
        (7, 2, 3, 4, 5),
    ],
)
def test_ndarray(dtype, shape, external):
    """Create np.array and put/get to/from flat HDC container
    """
    if dtype == np.bool_:
        x_in = np.random.randint(0, 2, size=shape, dtype=dtype)
    else:
        x_in = np.arange(np.prod(shape), dtype=dtype).reshape(shape)
    h = HDC()
    h.set_data(x_in, external=external)
    x_out = np.asarray(h)
    assert x_in.shape == x_out.shape
    assert x_in.size == x_out.size
    assert x_in.dtype == x_out.dtype
    assert x_in.strides == x_out.strides
    assert np.all(x_in == x_out)


@pytest.mark.parametrize("dtype", [np.float32, np.float64, np.int32, np.int64])
@pytest.mark.parametrize("shape", [(), (5,), (6, 8), (7, 2, 3)])
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
    assert h.shape == (len(test_list),)

    assert len(h) == len(test_list)

    # test iteration
    assert [x.to_python() for x in h] == test_list


def test_hdc_map_type():
    h = HDC()
    test_map = {"jedna": "one", "dva": "two"}
    h = HDC(test_map)
    assert test_map == json.loads(h.dumps())
    assert h.shape == (len(test_map),)

    assert len(h) == len(test_map)

    assert [k for k in h] == list(test_map.keys())

    with pytest.raises(KeyError):
        h["doesnotexist"]


def test_str_type():
    h = HDC()
    value = "test string"
    h = HDC(value)
    assert value == str(h)
    assert h.shape == (len(value),)


def test_keys():
    tree = HDC()
    keys = ["a", "b", "cc", "D"]
    for key in keys:
        tree[key] = "value"
    res = list(tree.keys())
    assert res == keys


def test_1():

    empty = HDC()
    assert empty.dumps().strip() == "null"

    s = HDC("Ahoj")
    print(s.dumps())

    tree = HDC()
    tree["a"] = "a"
    tree["sub/sub/x"] = "x"
    assert "a" in tree
    assert "sub" in tree
    assert "A" not in tree
    assert "SUB" not in tree
    # assert tree['sub']['sub'] == tree['sub/sub']


def random_array(low=0, high=1e10, rank=1, maxsize=20, dtype=np.float_):
    # random size and data
    size = np.random.random_integers(1, maxsize, rank)
    a, b = np.random.random_integers(low, high, size=(2,))
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


def test_to_python():
    tree = HDC()
    tree["root/none"] = None
    tree["root/str"] = "string"
    tree["root/int"] = 1
    tree["root/float"] = 1.1
    hdc_list = HDC()
    hdc_list.append("one")
    hdc_list.append(2)
    tree["root/list"] = hdc_list
    tree["root/numpy"] = np.arange(10)

    pytree = tree.to_python()
    pytree_test = {
        "root": {
            "float": 1.1,
            "int": 1,
            "list": ["one", 2],
            "str": "string",
            "none": None,
        }
    }

    assert all(pytree["root"]["numpy"] == np.arange(10))
    del pytree["root"]["numpy"]
    assert pytree == pytree_test


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


@pytest.mark.parametrize(
    "dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8, np.bool_]
)
@pytest.mark.parametrize(
    "shape",
    [
        (),
        (1,),
        (5,),
        (1, 1),
        (1, 3),
        (4, 1),
        (6, 8),
        (1, 3, 4),
        (7, 2, 3),
        (1, 2, 3, 4),
        (7, 2, 3, 4),
        (1, 2, 3, 4, 5),
        (7, 2, 3, 4, 5),
    ],
)
def test_ndarray_to_hdf5(dtype, shape):
    """Create np.array and put/get to/from flat HDC container
    """
    import h5py

    if dtype == np.bool_:
        x_in = np.random.randint(0, 2, size=shape, dtype=dtype)
    else:
        x_in = np.arange(np.prod(shape), dtype=dtype).reshape(shape)
    h = HDC()
    h.set_data(x_in)
    with tempfile.NamedTemporaryFile(suffix=".h5") as tmppfile:
        h.to_hdf5(tmppfile.name, dataset_name="data")
        with h5py.File(tmppfile.name, mode="r") as h5file:
            x_out = np.asarray(h5file["data"])
            assert x_in.shape == x_out.shape
            assert x_in.size == x_out.size
            # assert x_in.dtype == x_out.dtype
            assert x_in.strides == x_out.strides
            assert np.all(x_in == x_out)


@pytest.mark.parametrize(
    "dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8, np.bool_]
)
@pytest.mark.parametrize(
    "shape",
    [
        (),
        (1,),
        (5,),
        (1, 1),
        (1, 3),
        (4, 1),
        (6, 8),
        (1, 3, 4),
        (7, 2, 3),
        (1, 2, 3, 4),
        (7, 2, 3, 4),
        (1, 2, 3, 4, 5),
        (7, 2, 3, 4, 5),
    ],
)
def test_ndarray_from_hdf5(dtype, shape):
    """Create np.array and put/get to/from flat HDC container
    """
    import h5py

    if dtype == np.bool_:
        x_in = np.random.randint(0, 2, size=shape, dtype=dtype)
    else:
        x_in = np.arange(np.prod(shape), dtype=dtype).reshape(shape)
    with tempfile.NamedTemporaryFile(suffix=".h5") as tmppfile:
        with h5py.File(tmppfile.name, mode="w") as h5file:
            h5file["data"] = x_in
            h5file.close()
            h = HDC.from_hdf5(tmppfile.name, dataset_name="data")

            x_out = np.asarray(h)
            assert x_in.shape == x_out.shape
            assert x_in.size == x_out.size
            # assert x_in.dtype == x_out.dtype
            assert x_in.strides == x_out.strides
            assert np.all(x_in == x_out)


@pytest.mark.parametrize(
    "dtype", [np.float32, np.float64, np.int32, np.int64, np.int16, np.int8, np.bool_]
)
@pytest.mark.parametrize(
    "shape",
    [
        (),
        (1,),
        (5,),
        (1, 1),
        (1, 3),
        (4, 1),
        (6, 8),
        (1, 3, 4),
        (7, 2, 3),
        (1, 2, 3, 4),
        (7, 2, 3, 4),
        (1, 2, 3, 4, 5),
        (7, 2, 3, 4, 5),
    ],
)
def test_ndarray_json_string(dtype, shape):
    """Create np.array and put/get to/from flat HDC container
    """
    if dtype == np.bool_:
        x_in = np.random.randint(0, 2, size=shape, dtype=dtype)
    else:
        x_in = np.arange(np.prod(shape), dtype=dtype).reshape(shape)

    hdc = HDC(x_in)
    hdc.dump("save.json")
    json_string = hdc.dumps()
    h = HDC.loads(json_string)
    h.dump("load.json")
    x_out = np.asarray(h)
    assert x_in.shape == x_out.shape
    assert x_in.size == x_out.size
    # JSON currently does not save dtypes
    np.issubdtype(x_in.dtype, x_out.dtype)
    assert np.all(x_in == x_out)


def test_tree_hdf5(test_trees):
    pytree, hdctree = test_trees
    with tempfile.NamedTemporaryFile(suffix=".h5") as tmppfile:
        hdctree.to_hdf5(tmppfile.name)
        hdctree_test = HDC.from_hdf5(tmppfile.name)

    assert tree_equal(pytree, hdctree_test, exception=False)


def test_tree_json(test_trees):
    pytree, hdctree = test_trees
    with tempfile.NamedTemporaryFile(suffix=".json") as tmpfile:
        hdctree.dump(tmpfile.name)
        hdctree.dump("aaa.json")
        hdctree_test = HDC.load("json://" + tmpfile.name)
        print(pytree)
        print(hdctree_test.dumps())
        assert tree_equal(pytree, hdctree_test, exception=False)


def test_tree_json_string(test_trees):
    pytree, hdctree = test_trees
    json_str = hdctree.dumps()
    hdctree_test = HDC.loads(json_str)
    assert tree_equal(pytree, hdctree_test, exception=False)


if __name__ == "__main__":
    pytest.main()
