import numpy as np
from pyhdc import HDC
import random
import string


def test_put_ndarray():
    # loop over number of dimensions
    for ndim in (1, ):
        # loop over types
        for dtype in (np.float_, np.int32, ):
            # create a new container
            tree = HDC()
            # random size and data
            size = np.random.random_integers(1, 20, ndim)
            a, b = np.random.random_integers(0, 1e10, size=(2, ))
            pydata = ((b - a) * np.random.random(size=size) + a).astype(dtype)
            # store data into the container with a random key
            key = "".join((random.choice(string.ascii_letters) for _ in range(5)))
            tree[key] = pydata
            # copy numpy data
            copydata = pydata.copy()
            # and delete the original numpy
            del pydata
            # get the data back from the container
            data = tree[key].as_array()
            # compare the data
            assert np.all(copydata == data)
            assert copydata.dtype == data.dtype
