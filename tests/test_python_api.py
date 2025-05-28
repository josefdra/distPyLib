import pytest
import sys
import os
import atexit

# Add the build directory to path - try multiple locations
possible_paths = [
    os.path.join(os.path.dirname(__file__), '..', 'build'),
    os.path.join(os.path.dirname(__file__), '..', 'build', 'lib'),
    os.path.join(os.path.dirname(__file__), '..'),  # If installed with pip -e .
]

for path in possible_paths:
    if path not in sys.path:
        sys.path.insert(0, path)

from distPyLib import MPIComputer

# Global computer instance to avoid multiple MPI initializations
_global_computer = None

def get_computer():
    """Get a singleton MPI computer instance."""
    global _global_computer
    if _global_computer is None:
        _global_computer = MPIComputer(auto_spawn=False)
    return _global_computer

def test_mpi_computer_creation():
    """Test that MPIComputer can be created."""
    computer = get_computer()
    assert computer is not None

def test_mpi_initialization():
    """Test MPI initialization."""
    computer = get_computer()
    assert computer.rank >= 0
    assert computer.size >= 1

def test_parallel_computation():
    """Test parallel computation."""
    computer = get_computer()
    data = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
    results = computer.compute(data)
    
    assert len(results) == computer.size
    assert all(isinstance(r, float) for r in results)
    
    # Verify the computation makes sense
    # Each process should have computed something > 0
    assert all(r > 0 for r in results)

if __name__ == "__main__":
    pytest.main([__file__, "-v"])
    