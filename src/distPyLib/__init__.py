"""
MPI Library - A Python package with C++ MPI backend
"""

from .core import cc_mpi_test, cc_collective_operation

__version__ = "1.0.0"
__all__ = ["cc_mpi_test", "cc_collective_operation"]
