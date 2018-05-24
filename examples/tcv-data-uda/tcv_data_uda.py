import collections
import ctypes
import numpy as np

from pyhdc import HDC


MDS_SERVER = 'localhost:8001'
URI_TMPL = r'uda://{signal}|MDSPLUS::{mds_server}/tcv_shot/{shot}'
FORTRAN = ctypes.cdll.LoadLibrary('process_tcv_data.so')


def get_data(signal, shot, mds_server=MDS_SERVER):
    uri = URI_TMPL.format(signal=signal, shot=shot, mds_server=mds_server)
    return HDC(uri)


def get_data_tree(tree_meta, shot):
    res = HDC()
    for key, value in tree_meta.items():
        if isinstance(value, collections.Mapping):
            res[key] = get_data_tree(value, shot)
        else:
            res[key] = get_data(value, shot)
    return res


if __name__ == '__main__':

    tree_meta = {
        'Ip': {
            'data': r'\magnetics::iplasma:trapeze',
            'time': r'dim_of(\magnetics::iplasma:trapeze)',
        },
        'lcfs': {
            'R': r'\r_contour',
            'Z': r'\z_contour',
            'time': r'dim_of(\results::r_axis)',
        },
        'Te': {
            'data': r'\results::proffit.avg_time:teft',
            'time': r'dim_of(\results::proffit.avg_time:teft)',
        }
    }

    tree = get_data_tree(tree_meta, 60950)

    Ip = np.asarray(tree['Ip/data'])
    # save a copy
    Ip_copy = Ip.copy()

    tree_out = HDC()
    # the Fortran wrapper must be called with output tree_out already allocated
    FORTRAN.process_data(tree, tree_out)
    # let's check the results
    assert tree_out['Ip/min'] == Ip.min()
    assert tree_out['Ip/max'] == Ip.max()

    # demonstrate that we are sharing the memory across HDC / Numpy / Fortran
    FORTRAN.process_data_inplace(tree)
    assert Ip.max() == 1
    assert Ip.min() == 0
    assert Ip_copy.min() == tree_out['Ip/min']
    assert Ip_copy.max() == tree_out['Ip/max']
    assert np.allclose((Ip_copy - Ip_copy.min()) / (Ip_copy.max() - Ip_copy.min()), Ip)
