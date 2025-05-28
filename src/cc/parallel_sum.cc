#include "parallel_sum.hh"

ParaSum::ParaSum() : mpi_initialized_by_us_(false), rank(0), size(1), 
                     intercomm_(MPI_COMM_NULL), has_spawned_processes_(false) {
  ensure_mpi_initialized();
}

ParaSum::~ParaSum() {
  cleanup_spawned_processes();
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

void ParaSum::initialize() { 
  ensure_mpi_initialized(); 
}

void ParaSum::spawn_worker_processes(int num_processes) {
  if (has_spawned_processes_) {
    cleanup_spawned_processes();
  }

  // Create a simple worker program name - you'll need to create this executable
  const char* worker_program = "mpi_worker";
  
  // Spawn the worker processes
  int error_code = MPI_Comm_spawn(
    worker_program,           // program to spawn
    MPI_ARGV_NULL,           // arguments
    num_processes,           // number of processes to spawn
    MPI_INFO_NULL,           // info object
    0,                       // root rank
    MPI_COMM_SELF,           // parent communicator
    &intercomm_,             // intercommunicator
    MPI_ERRCODES_IGNORE      // error codes
  );

  if (error_code != MPI_SUCCESS) {
    throw std::runtime_error("Failed to spawn MPI worker processes");
  }

  has_spawned_processes_ = true;
}

void ParaSum::cleanup_spawned_processes() {
  if (has_spawned_processes_ && intercomm_ != MPI_COMM_NULL) {
    // Send shutdown signal to workers
    int shutdown_signal = -1;
    MPI_Bcast(&shutdown_signal, 1, MPI_INT, MPI_ROOT, intercomm_);
    
    // Disconnect from spawned processes
    MPI_Comm_disconnect(&intercomm_);
    has_spawned_processes_ = false;
  }
}

double ParaSum::parallel_sum(const std::vector<double> &data, int num_processes) {
  ensure_mpi_initialized();
  
  // Spawn worker processes if not already done
  if (!has_spawned_processes_) {
    spawn_worker_processes(num_processes);
  }

  // Send data size to workers
  int data_size = static_cast<int>(data.size());
  MPI_Bcast(&data_size, 1, MPI_INT, MPI_ROOT, intercomm_);

  // Send the actual data to workers
  MPI_Bcast(const_cast<double*>(data.data()), data_size, MPI_DOUBLE, MPI_ROOT, intercomm_);

  // Receive partial sums from workers
  std::vector<double> partial_sums(num_processes);
  MPI_Gather(nullptr, 0, MPI_DOUBLE, 
             partial_sums.data(), 1, MPI_DOUBLE, 
             MPI_ROOT, intercomm_);

  // Calculate total sum
  double total_sum = 0.0;
  for (double partial : partial_sums) {
    total_sum += partial;
  }

  return total_sum;
}

int ParaSum::get_rank() const { return rank; }

int ParaSum::get_size() const { return size; }

bool ParaSum::is_initialized() const {
  int initialized;
  MPI_Initialized(&initialized);
  return initialized != 0;
}
