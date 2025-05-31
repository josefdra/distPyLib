cmake_minimum_required(VERSION 3.20)
project(distPyLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(MPI REQUIRED)
find_package(pybind11 REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/cc)

# Create the C++ library
add_library(mpi_test STATIC
    src/cc/mpi_test.cc
)

target_link_libraries(mpi_test 
    MPI::MPI_CXX
)

target_include_directories(mpi_test PUBLIC
    ${CMAKE_SOURCE_DIR}/src/cc
    ${MPI_CXX_INCLUDE_DIRS}
)

# Create Python module
pybind11_add_module(_mpi_test 
    src/cc/python_bindings.cc
)

target_link_libraries(_mpi_test PRIVATE 
    mpi_test
    MPI::MPI_CXX
)

# Compiler-specific options
target_compile_definitions(_mpi_test PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

# Install Python module and executable
install(TARGETS _mpi_test DESTINATION distPyLib)

# Enable testing
enable_testing()
add_subdirectory(tests/cc)

