#include "parallel_sum.hh"

ParaSum::ParaSum() : mpi_initialized_by_us_(false), rank(0), size(1) {
  ensure_mpi_initialized();
}

ParaSum::~ParaSum() {
  if (mpi_initialized_by_us_) {
    MPI_Finalize();
  }
}

void ParaSum::ensure_mpi_initialized() {
  int initialized;
  MPI_Initialized(&initialized);

  if (!initialized) {
    MPI_Init(nullptr, nullptr);
    mpi_initialized_by_us_ = true;
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
}

void ParaSum::initialize() { ensure_mpi_initialized(); }

double ParaSum::parallel_sum(const std::vector<double> &data) {
  ensure_mpi_initialized();

  // Simple example: each process computes sum of its portion
  size_t local_size = data.size() / size;
  size_t start_idx = rank * local_size;
  size_t end_idx = (rank == size - 1) ? data.size() : start_idx + local_size;

  double local_sum = 0.0;
  for (size_t i = start_idx; i < end_idx; ++i) {
    local_sum += data[i];
  }

  // Gather results
  double all_sums;
  MPI_Allgather(&local_sum, 1, MPI_DOUBLE, &all_sums, 1, MPI_DOUBLE,
                MPI_COMM_WORLD);

  return all_sums;
}

int ParaSum::get_rank() const { return rank; }

int ParaSum::get_size() const { return size; }

bool ParaSum::is_initialized() const {
  int initialized;
  MPI_Initialized(&initialized);
  return initialized != 0;
}
