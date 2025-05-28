#include <mpi.h>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    MPI_Comm parent_comm;
    MPI_Comm_get_parent(&parent_comm);
    
    if (parent_comm == MPI_COMM_NULL) {
        std::cerr << "This program should be spawned by MPI_Comm_spawn" << std::endl;
        MPI_Finalize();
        return 1;
    }
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    while (true) {
        // Receive data size from parent
        int data_size;
        MPI_Bcast(&data_size, 1, MPI_INT, 0, parent_comm);
        
        // Check for shutdown signal
        if (data_size == -1) {
            break;
        }
        
        // Receive data from parent
        std::vector<double> data(data_size);
        MPI_Bcast(data.data(), data_size, MPI_DOUBLE, 0, parent_comm);
        
        // Calculate this worker's portion
        size_t chunk_size = data_size / size;
        size_t start_idx = rank * chunk_size;
        size_t end_idx = (rank == size - 1) ? data_size : start_idx + chunk_size;
        
        double partial_sum = 0.0;
        for (size_t i = start_idx; i < end_idx; ++i) {
            partial_sum += data[i];
        }
        
        // Send partial sum back to parent
        MPI_Gather(&partial_sum, 1, MPI_DOUBLE, 
                   nullptr, 0, MPI_DOUBLE, 
                   0, parent_comm);
    }
    
    MPI_Finalize();
    return 0;
}
