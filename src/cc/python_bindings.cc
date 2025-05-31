#include "mpi_test.hh"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(_mpi_test, m) {
    m.doc() = "Test communication of a c++ MPI library that can be called out of "
              "a python MPI context";

    m.def("cc_mpi_test", &cc_mpi_test, "Test communication",
          py::arg("python_rank"), py::arg("python_size"));
    
    m.def("cc_collective_operation", [](py::array_t<double> data, int data_size) {
        py::buffer_info buf = data.request();
        return cc_collective_operation(static_cast<double*>(buf.ptr), data_size);
    }, "Test collective operation", py::arg("data"), py::arg("data_size"));
}
