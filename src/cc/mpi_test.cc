#include "mpi_test.hh"

#include <iostream>
#include <mpi.h>
#include <vector>

void cc_mpi_test(int python_rank, int python_size) {
  int rank, size;

  // Get MPI rank and size (should match Python's values)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::cout << "C++: Rank " << rank << "/" << size
            << " (Python reported: " << python_rank << "/" << python_size << ")"
            << std::endl;

  // Verify that C++ and Python see the same MPI state
  if (rank != python_rank || size != python_size) {
    std::cerr << "ERROR: MPI state mismatch between Python and C++!"
              << std::endl;
    return;
  }

  // Perform some MPI communication
  if (rank == 0) {
    // Root process sends messages to all other ranks
    for (int i = 1; i < size; i++) {
      int message = 100 + i;
      MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      std::cout << "C++: Rank 0 sent " << message << " to rank " << i
                << std::endl;
    }

    // Receive responses
    for (int i = 1; i < size; i++) {
      int response;
      MPI_Recv(&response, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::cout << "C++: Rank 0 received " << response << " from rank " << i
                << std::endl;
    }
  } else {
    // Non-root processes receive message and send response
    int message;
    MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "C++: Rank " << rank << " received " << message
              << " from rank 0" << std::endl;

    int response = message * rank;
    MPI_Send(&response, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    std::cout << "C++: Rank " << rank << " sent response " << response
              << " to rank 0" << std::endl;
  }

  // Synchronize all ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // Perform a ring communication pattern
  int left_neighbor = (rank - 1 + size) % size;
  int right_neighbor = (rank + 1) % size;

  int send_data = rank * 10;
  int recv_data;

  // Send to right, receive from left
  MPI_Sendrecv(&send_data, 1, MPI_INT, right_neighbor, 2, &recv_data, 1,
               MPI_INT, left_neighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  std::cout << "C++: Rank " << rank << " sent " << send_data << " to rank "
            << right_neighbor << ", received " << recv_data << " from rank "
            << left_neighbor << std::endl;

  MPI_Barrier(MPI_COMM_WORLD);
}

double cc_collective_operation(double *data, int data_size) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Calculate local sum
  double local_sum = 0.0;
  for (int i = 0; i < data_size; i++) {
    local_sum += data[i];
  }

  std::cout << "C++: Rank " << rank << " local sum: " << local_sum << std::endl;

  // Perform MPI_Allreduce to get global sum
  double global_sum;
  MPI_Allreduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM,
                MPI_COMM_WORLD);

  // Perform MPI_Allgather to collect all local sums
  std::vector<double> all_local_sums(size);
  MPI_Allgather(&local_sum, 1, MPI_DOUBLE, all_local_sums.data(), 1, MPI_DOUBLE,
                MPI_COMM_WORLD);

  if (rank == 0) {
    std::cout << "C++: All local sums: ";
    for (int i = 0; i < size; i++) {
      std::cout << all_local_sums[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "C++: Global sum: " << global_sum << std::endl;
  }

  // Return a rank-specific result based on global computation
  return global_sum / size + rank;
}
