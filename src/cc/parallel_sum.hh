#pragma once
#include <mpi.h>
#include <vector>

class ParaSum {
public:
    ParaSum();
    ~ParaSum();
    
    // Initialize MPI if not already initialized
    void initialize();
    
    // Execute a function across spawned MPI processes
    double parallel_sum(const std::vector<double>& data, int num_processes = 4);
    
    // Get MPI info
    int get_rank() const;
    int get_size() const;
    bool is_initialized() const;
    
private:
    bool mpi_initialized_by_us_;
    int rank;
    int size;
    MPI_Comm intercomm_;  // Intercommunicator for spawned processes
    bool has_spawned_processes_;
    
    void ensure_mpi_initialized();
    void spawn_worker_processes(int num_processes);
    void cleanup_spawned_processes();
};
