import pytest
import sys
import os

# Add the build directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'build'))

from distPyLib import MPIComputer

def test_mpi_computer_creation():
    """Test that MPIComputer can be created."""
    computer = MPIComputer(auto_spawn=False)
    assert computer is not None

def test_mpi_initialization():
    """Test MPI initialization."""
    computer = MPIComputer(auto_spawn=False)
    assert computer.rank >= 0
    assert computer.size >= 1

def test_parallel_computation():
    """Test parallel computation."""
    computer = MPIComputer(auto_spawn=False)
    data = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
    results = computer.compute(data)
    
    assert len(results) == computer.size
    assert all(isinstance(r, float) for r in results)

if __name__ == "__main__":
    pytest.main([__file__])
    