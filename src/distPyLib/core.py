import numpy as np
try:
    from . import _mpi_test
except ImportError:
    import _mpi_test # type: ignore

def cc_mpi_test(python_rank: int, python_size: int) -> None:
    """Tests general MPI functionality

    Args:
        python_rank (int): The rank of the parent python program
        python_size (int): The world size of the parent python programs MPI context
    """
    return _mpi_test.cc_mpi_test(python_rank, python_size)

def cc_collective_operation(data: np.ndarray, data_size: int) -> float:
    """Tests general collective operations of MPI like send, recv, allGather and allReduce

    Args:
        data (ctypes.POINTER): Pointer to test data
        data_size (int): number of elements in test data

    Returns:
        float: result of collective operations
    """
    return _mpi_test.cc_collective_operation(data, data_size)
    