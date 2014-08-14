
#include <stdio.h>
#include "/afs/slac.stanford.edu/package/OpenMPI/include/mpi.h"


int main(int argc, char *argv[]) {
  int numprocs, rank, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);

  printf("Process %d on %s out of %d\n", rank, processor_name, numprocs);

  MPI_Finalize();
} 


// # Linux with Ethernet (OpenMPI):
// /afs/slac.stanford.edu/package/OpenMPI/bin/mpicc hello.c -o hello
// Open MPI is using the following environment variables for alternative compilers:
// C: OMPI_MPICC
// C++: OMPI_MPIXX Fortran 77: OMPI_MPIF77
// Fortran 90: OMPI_MPIF90

// bsub -q mpitestq -a openmpi -n 10 hello.exe
