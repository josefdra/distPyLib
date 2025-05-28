import os
import sys
from typing import List, Optional
try:
    from . import _mpi_core
except ImportError:
    import _mpi_core

class MPIComputer:
    """High-level interface for MPI computations."""
    
    def __init__(self, auto_spawn: bool = True, num_processes: Optional[int] = None):
        self.auto_spawn = auto_spawn
        self.num_processes = num_processes or os.cpu_count()
        self._wrapper = None
        self._ensure_mpi_environment()
    
    def _ensure_mpi_environment(self):
        """Ensure we're running in an MPI environment."""
        # Check if we're already in an MPI environment
        if 'OMPI_COMM_WORLD_SIZE' in os.environ or 'PMI_SIZE' in os.environ:
            # Already in MPI context
            self._wrapper = _mpi_core.MPIWrapper()
            return
        
        if self.auto_spawn and self._wrapper is None:
            # Need to restart with mpirun
            self._restart_with_mpi()
    
    def _restart_with_mpi(self):
        """Restart the current script with mpirun."""
        if len(sys.argv) > 0 and not sys.argv[0].endswith('pytest'):
            mpi_cmd = [
                'mpirun', '-n', str(self.num_processes),
                sys.executable
            ] + sys.argv
            
            print(f"Restarting with MPI: {' '.join(mpi_cmd)}")
            os.execvp('mpirun', mpi_cmd)
        else:
            # In testing or interactive mode, just create wrapper
            self._wrapper = _mpi_core.MPIWrapper()
    
    def compute(self, data: List[float]) -> List[float]:
        """Run parallel computation on data."""
        if self._wrapper is None:
            self._wrapper = _mpi_core.MPIWrapper()
        
        return self._wrapper.parallel_compute(data)
    
    @property
    def rank(self) -> int:
        """Get MPI rank."""
        if self._wrapper is None:
            self._wrapper = _mpi_core.MPIWrapper()
        return self._wrapper.get_rank()
    
    @property
    def size(self) -> int:
        """Get MPI size."""
        if self._wrapper is None:
            self._wrapper = _mpi_core.MPIWrapper()
        return self._wrapper.get_size()
    
    @property
    def is_mpi_initialized(self) -> bool:
        """Check if MPI is initialized."""
        if self._wrapper is None:
            return False
        return self._wrapper.is_initialized()
    