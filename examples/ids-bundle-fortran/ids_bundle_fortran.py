import ctypes
import numpy as np
import os

from pyhdc import HDC


SHOT = 130501
RUN = 1


def get_data_tree(shot, run, cache=True):
    # get input data
    cache_file = "hdc_cache_{}_{}.h5".format(shot, run)
    if cache and os.path.isfile(cache_file):
        # load from cache
        print('loading from {}'.format(cache_file))
        ids_bundle = HDC.from_hdf5(cache_file)
    else:
        # readl from IMAS if cache file does not exist
        import pyual

        shot = SHOT
        run = RUN

        cli = pyual.Client(shot, run, user='g2jurban', machine='iter')

        coreprof = cli.get('core_profiles')
        equi = cli.get('equilibrium')
        pf_active = cli.get('pf_active')
        pf_passive = cli.get('pf_passive')
        wall = cli.get('wall')

        ids_bundle = HDC()

        ids_bundle['coreprof'] = pyual.cpo2dict(coreprof, cls=HDC)
        ids_bundle['equi'] = pyual.cpo2dict(equi, cls=HDC)
        ids_bundle['pf_active'] = pyual.cpo2dict(pf_active, cls=HDC)
        ids_bundle['pf_passive'] = pyual.cpo2dict(pf_passive, cls=HDC)
        ids_bundle['wall'] = pyual.cpo2dict(wall, cls=HDC)

        if cache:
            ids_bundle.to_hdf5(cache_file)

    return ids_bundle


if __name__ == '__main__':

    ids_bundle = get_data_tree(shot=SHOT, run=RUN)
    fortran_lib = ctypes.cdll.LoadLibrary('./process_data.so')

    # the Fortran wrapper must be called with output tree_out already allocated
    tree_out = HDC()
    fortran_lib.process_data(ids_bundle, tree_out)

    # let's check the results
    time = np.asarray(ids_bundle['coreprof/time'])
    it = 30
    prof = ids_bundle['coreprof/profiles_1d'][it]
    ti = np.asarray(prof['t_i_average'])
    te = np.asarray(prof['electrons/temperature'])
    tite = ti / te

    assert np.allclose(tite, np.asarray(tree_out['output/tite']))
