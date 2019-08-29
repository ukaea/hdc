#/bin/bash
HDC_PREFIX=../../install
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HDC_PREFIX/lib:HDC_PREFIX/lib/hdc mpirun -n 4 ./hdc_mpi_send_recv
