#!/usr/bin/env python3
from distPyLib import ParaSum

def main():
    # No more automatic mpirun restart - runs as single Python process
    paraSum = ParaSum(num_processes=4)
    
    print(f"Python process running on rank {paraSum.rank} of {paraSum.size}")
    print("(This should always be rank 0 of 1)")
    
    # Some data to process
    data = list(range(1, 13))  # [1, 2, 3, ..., 12]
    print(f"Input data: {data}")
    
    # Run parallel computation (C++ will spawn MPI processes internally)
    print("Starting parallel computation...")
    result = paraSum.parallel_sum(data)
    
    print(f"Final sum: {result}")
    print("Done!")

if __name__ == "__main__":
    main()
    