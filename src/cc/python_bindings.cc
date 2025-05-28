#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "mpi_wrapper.hh"

namespace py = pybind11;

PYBIND11_MODULE(_mpi_core, m) {
    m.doc() = "MPI wrapper for Python";
    
    py::class_<MPIWrapper>(m, "MPIWrapper")
        .def(py::init<>())
        .def("initialize", &MPIWrapper::initialize, "Initialize MPI")
        .def("parallel_compute", &MPIWrapper::parallel_compute, "Run parallel computation")
        .def("get_rank", &MPIWrapper::get_rank, "Get MPI rank")
        .def("get_size", &MPIWrapper::get_size, "Get MPI size")
        .def("is_initialized", &MPIWrapper::is_initialized, "Check if MPI is initialized")
        .def("spawn_processes", &MPIWrapper::spawn_processes, "Spawn MPI processes");
}
