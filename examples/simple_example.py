#!/usr/bin/env python3
from distPyLib import ParaSum

def main():
    # This will automatically restart with mpirun if needed
    paraSum = ParaSum(num_processes=4)
    
    print(f"Running on rank {paraSum.rank} of {paraSum.size}")
    
    # Some data to process
    data = list(range(1, 13))  # [1, 2, 3, ..., 12]
    print(f"Input data: {data}")
    
    # Run parallel computation
    results = paraSum.parallel_sum(data)
    
    if paraSum.rank == 0:  # Only print from rank 0
        print(f"Results from all processes: {results}")
        print("Done!")

if __name__ == "__main__":
    main()
    