EXECS=hdc_mpi_send_recv
MPICC?=mpicc
MPICXX?=mpic++
HDC_ROOT?=$(realpath ../../install)
FLAGS=-std=c++11 -I${HDC_ROOT}/include/hdc -L${HDC_ROOT}/lib -lhdc -lboost_system

all: ${EXECS}

hdc_mpi_send_recv: hdc_mpi_send_recv.c
	${MPICXX} -o hdc_mpi_send_recv hdc_mpi_send_recv.c ${FLAGS}

clean:
	rm -f ${EXECS}
