import ctypes
try:
    from . import _mpi_test
except ImportError:
    import _mpi_test # type: ignore

def cc_mpi_test(python_rank: int, python_size: int):
    return _mpi_test.cc_mpi_test(python_rank, python_size)

def cc_collective_operation(data: ctypes.POINTER, data_size: int) -> float:
    return _mpi_test.cc_collective_operation(data, data_size)
    