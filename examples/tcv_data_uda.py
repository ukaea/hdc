import collections

from pyhdc import HDC


MDS_SERVER = 'localhost:8001'
URI_TMPL = r'uda://{signal}|MDSPLUS::{mds_server}/tcv_shot/{shot}'


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
