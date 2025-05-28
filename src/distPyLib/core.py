import os
from typing import List, Optional
from pathlib import Path
try:
    from . import _parallel_sum
except ImportError:
    import _parallel_sum

class ParaSum:
    """High-level interface for MPI computations."""
    
    def __init__(self, num_processes: Optional[int] = None):
        self.num_processes = num_processes or os.cpu_count()
        
        # Set worker path environment variable for C++ to find executable
        worker_path = Path(__file__).parent / "mpi_worker"
        if worker_path.exists():
            os.environ["PATH"] = str(worker_path.parent) + os.pathsep + os.environ.get("PATH", "")
            print("found")
        else:
            print("not found")
        
        self.paraSum = _parallel_sum.ParaSum()
    
    def parallel_sum(self, data: List[float]) -> float:
        """Run parallel summation on data."""
        return self.paraSum.parallel_sum(data, self.num_processes)
    
    @property
    def rank(self) -> int:
        """Get MPI rank (always 0 for the Python process)."""
        return self.paraSum.get_rank()
    
    @property
    def size(self) -> int:
        """Get MPI size (always 1 for the Python process)."""
        return self.paraSum.get_size()
    
    @property
    def is_mpi_initialized(self) -> bool:
        """Check if MPI is initialized."""
        return self.paraSum.is_initialized()
    