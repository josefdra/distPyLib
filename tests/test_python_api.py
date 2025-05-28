import pytest
import sys
import os
import atexit

from distPyLib import ParaSum

_para_sum = None

def get_para_sum():
    """Get a singleton MPI sum instance."""
    global _para_sum
    if _para_sum is None:
        _para_sum = ParaSum(auto_spawn=False)
    return _para_sum

def test_mpi_computer_creation():
    """Test that MPIComputer can be created."""
    para_sum = get_para_sum()
    assert para_sum is not None

def test_mpi_initialization():
    """Test MPI initialization."""
    para_sum = get_para_sum()
    assert para_sum.rank >= 0
    assert para_sum.size >= 1

def test_parallel_computation():
    """Test parallel computation."""
    para_sum = get_para_sum()
    data = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
    result = para_sum.parallel_sum(data)
    
    assert isinstance(result, float)
    assert result == 21.0

if __name__ == "__main__":
    pytest.main([__file__, "-v"])
    