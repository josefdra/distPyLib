#!/usr/bin/env python3
from distPyLib import MPIComputer

def main():
    # This will automatically restart with mpirun if needed
    computer = MPIComputer(num_processes=4)
    
    print(f"Running on rank {computer.rank} of {computer.size}")
    
    # Some data to process
    data = list(range(1, 13))  # [1, 2, 3, ..., 12]
    print(f"Input data: {data}")
    
    # Run parallel computation
    results = computer.compute(data)
    
    if computer.rank == 0:  # Only print from rank 0
        print(f"Results from all processes: {results}")
        print("Done!")

if __name__ == "__main__":
    main()
    