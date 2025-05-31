#!/usr/bin/env python3
"""
Test script that initializes MPI across 4 ranks in Python,
then calls a C++ library that uses the same MPI communicator.
"""

import sys
from mpi4py import MPI
import ctypes
import numpy as np
import distPyLib

def main():
    # Initialize MPI
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    
    print(f"Python: Rank {rank}/{size} initialized")
    
    # Ensure we have exactly 4 ranks
    if size != 4:
        if rank == 0:
            print(f"Error: Expected 4 ranks, got {size}")
            print("Run with: mpirun -n 4 python mpi_test.py")
        sys.exit(1)
    
    # Synchronize before calling C++ functions
    comm.Barrier()
    
    print(f"Python: Rank {rank} calling C++ library...")
    
    # Call C++ function that performs MPI communication
    distPyLib.cc_mpi_test(rank, size)
    
    # Test collective operation with data
    # Create some test data
    local_data = np.array([float(rank + 1), float(rank * 2)], dtype=np.float64)
    
    print(f"python script, rank {rank}, local data before collective operation: {local_data}")
    
    # Call C++ collective operation
    result = distPyLib.cc_collective_operation(local_data, len(local_data))
    
    print(f"python script, rank {rank},  local data after collective operation: {local_data}")
    
    print(f"Python: Rank {rank} - C++ collective result: {result}")
    
    # Final synchronization
    comm.Barrier()
    
    if rank == 0:
        print("Python: All ranks completed successfully!")

if __name__ == "__main__":
    main()
    