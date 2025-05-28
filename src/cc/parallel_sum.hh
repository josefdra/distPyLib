#pragma once
#include <mpi.h>
#include <vector>

class ParaSum {
public:
    ParaSum();
    ~ParaSum();
    
    // Initialize MPI if not already initialized
    void initialize();
    
    // Execute a function across all MPI processes
    double parallel_sum(const std::vector<double>& data);
    
    // Get MPI info
    int get_rank() const;
    int get_size() const;
    bool is_initialized() const;
    
private:
    bool mpi_initialized_by_us_;
    int rank;
    int size;
    
    void ensure_mpi_initialized();
};
