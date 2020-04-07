#!/bin/bash
make clean
make
python setup.py build_ext -i
PYTHONPATH=$PYTHONPATH. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. python test_hdcdumper.py
