EXECS=hdc_mpi_send_recv
MPICC?=mpicc
MPICXX?=mpic++
FLAGS=-std=c++11 -I/home/david/projects/hdc/install/include/hdc -L/home/david/projects/hdc/install/lib -lhdc -lboost_system

all: ${EXECS}

hdc_mpi_send_recv: hdc_mpi_send_recv.c
	${MPICXX} -o hdc_mpi_send_recv hdc_mpi_send_recv.c ${FLAGS}

clean:
	rm -f ${EXECS}
