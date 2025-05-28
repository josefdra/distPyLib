#pragma once
#include <mpi.h>
#include <vector>

class MPIWrapper {
public:
    MPIWrapper();
    ~MPIWrapper();
    
    // Initialize MPI if not already initialized
    void initialize();
    
    // Execute a function across all MPI processes
    std::vector<double> parallel_compute(const std::vector<double>& data);
    
    // Get MPI info
    int get_rank() const;
    int get_size() const;
    bool is_initialized() const;
    
    // Automatic process spawning
    void spawn_processes(int num_processes);
    
private:
    bool mpi_initialized_by_us_;
    int rank_;
    int size_;
    
    void ensure_mpi_initialized();
};
