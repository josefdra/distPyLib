#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "parallel_sum.hh"

namespace py = pybind11;

PYBIND11_MODULE(_parallel_sum, m) {
    m.doc() = "Parallel summation with mpi for Python";
    
    py::class_<ParaSum>(m, "ParaSum")
        .def(py::init<>())
        .def("initialize", &ParaSum::initialize, "Initialize MPI")
        .def("parallel_sum", &ParaSum::parallel_sum, "Run parallel summation")
        .def("get_rank", &ParaSum::get_rank, "Get MPI rank")
        .def("get_size", &ParaSum::get_size, "Get MPI size")
        .def("is_initialized", &ParaSum::is_initialized, "Check if MPI is initialized");
}
