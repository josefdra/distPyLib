#include "mpi_wrapper.hh"
#include <iostream>

MPIWrapper::MPIWrapper() : mpi_initialized_by_us_(false), rank_(0), size_(1) {
    ensure_mpi_initialized();
}

MPIWrapper::~MPIWrapper() {
    if (mpi_initialized_by_us_) {
        MPI_Finalize();
    }
}

void MPIWrapper::ensure_mpi_initialized() {
    int initialized;
    MPI_Initialized(&initialized);
    
    if (!initialized) {
        MPI_Init(nullptr, nullptr);
        mpi_initialized_by_us_ = true;
    }
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &size_);
}

void MPIWrapper::initialize() {
    ensure_mpi_initialized();
}

std::vector<double> MPIWrapper::parallel_compute(const std::vector<double>& data) {
    ensure_mpi_initialized();
    
    // Simple example: each process computes sum of its portion
    size_t local_size = data.size() / size_;
    size_t start_idx = rank_ * local_size;
    size_t end_idx = (rank_ == size_ - 1) ? data.size() : start_idx + local_size;
    
    double local_sum = 0.0;
    for (size_t i = start_idx; i < end_idx; ++i) {
        local_sum += data[i] * 2.0; // Simple computation
    }
    
    // Gather results
    std::vector<double> all_sums(size_);
    MPI_Allgather(&local_sum, 1, MPI_DOUBLE, all_sums.data(), 1, MPI_DOUBLE, MPI_COMM_WORLD);
    
    return all_sums;
}

int MPIWrapper::get_rank() const {
    return rank_;
}

int MPIWrapper::get_size() const {
    return size_;
}

bool MPIWrapper::is_initialized() const {
    int initialized;
    MPI_Initialized(&initialized);
    return initialized != 0;
}

void MPIWrapper::spawn_processes(int num_processes) {
    // This is complex - for now, we'll assume MPI is launched externally
    // In a real implementation, you'd use MPI_Comm_spawn
    std::cout << "Spawning " << num_processes << " processes" << std::endl;
}
