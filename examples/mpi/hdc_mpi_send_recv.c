// Inspired by www.mpitutorial.com
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdc.hpp"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int trap_key = 0;

  // We have to use MDBM -- this works in shared memory only, for different nodes, we will have to add different storage backend (i.e. KVTree, redis, kelpie, mongodb ...)
  HDC::init("mdbm","{\"filename\": \"./db.mdbm\", \"persistent\": false}");
  hdc_t handle;
  auto size = sizeof(hdc_t);
  if (world_rank == 0) {
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Hello from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // Prepare some data
    int32_t array[4] = { 7, 2, 3, 4 };
    std::vector<size_t> shape = {4};
    // Add data to a single node
    HDC tree;
    tree["aaa/bbb/ccc"].set_data(shape, array);
    tree["string"].set_string("some string...");
    std::cout << "----------------------- HDC tree data -----------------------\n";
    tree.dump();
    std::cout << "-------------------------------------------------------------\n";
    // Let's get a handle
    handle = tree.as_obj();
    std::cout << "We will send just the following struct:\n";
    std::cout << "handle.storage_id: " << handle.storage_id << "\n";
    std::cout << "handle.uuid: " << uuid_to_str(handle.uuid) << "\n";
    for (int i=1; i<world_size; i++ ) {
        MPI_Send(
          /* data         = */ &handle,
          /* count        = */ size,
          /* datatype     = */ MPI_BYTE,
          /* destination  = */ i,
          /* tag          = */ 0,
          /* communicator = */ MPI_COMM_WORLD);
        std::cout << "--- handle sent to process " << i << std::endl;
    }
  } else if (world_rank >= 1) {
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_Recv(
      /* data         = */ &handle,
      /* count        = */ size,
      /* datatype     = */ MPI_BYTE,
      /* source       = */ 0,
      /* tag          = */ 0,
      /* communicator = */ MPI_COMM_WORLD,
      /* status       = */ MPI_STATUS_IGNORE);
    HDC received(handle);
    std::stringstream ss;

    ss << "*************************************************************" << std::endl
        << "Hello from processor " << processor_name << ", rank "
        << world_rank << " out of " << world_size << " processors\n" << std::endl
        << "handle received" << std::endl
        << "handle.storage_id: " << handle.storage_id << std::endl
        << "handle.uuid: " << uuid_to_str(handle.uuid) << std::endl
        << "----------------------- HDC tree data -----------------------" << std::endl
        << received.serialize("json") << std::endl
        << "-------------------------------------------------------------" << std::endl;
    std::cout << ss.str();
    MPI_Send(&trap_key, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  // HDC cleanup
  HDC::destroy();
}
