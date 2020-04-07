#!/bin/bash
HDC_INSTALL_PREFIX="../../install"
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:${HDC_INSTALL_PREFIX}/lib/pkgconfig
make clean
make
python setup.py build_ext -i
PYTHONPATH=$PYTHONPATH. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. python test_hdcdumper.py
