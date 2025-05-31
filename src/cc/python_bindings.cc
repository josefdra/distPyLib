#include "mpi_test.hh"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_MODULE(_mpi_test, m) {
  m.doc() = "Test communication of a c++ MPI library that can be called out of "
            "a python MPI context";

  m.def("cc_mpi_test", &cc_mpi_test, "Test communication");
  m.def("cc_collective_operation", &cc_collective_operation,
        "Test collective operation");
}
