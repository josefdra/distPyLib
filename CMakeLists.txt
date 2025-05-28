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
add_library(mpi_core STATIC
    src/cc/mpi_wrapper.cc
)

target_link_libraries(mpi_core 
    MPI::MPI_CXX
)

target_include_directories(mpi_core PUBLIC
    ${CMAKE_SOURCE_DIR}/src/cc
    ${MPI_CXX_INCLUDE_DIRS}
)

# Create Python module
pybind11_add_module(_mpi_core 
    src/cc/python_bindings.cc
)

target_link_libraries(_mpi_core PRIVATE 
    mpi_core
    MPI::MPI_CXX
)

# Compiler-specific options
target_compile_definitions(_mpi_core PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

# Enable testing
enable_testing()
add_subdirectory(tests/cc)

find_program(PYTHON_EXECUTABLE python3)
if(NOT PYTHON_EXECUTABLE)
    find_program(PYTHON_EXECUTABLE python)
endif()

if(PYTHON_EXECUTABLE)
    # Python tests
    add_test(
        NAME Python_Tests_Sequential
        COMMAND ${PYTHON_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/tests/test_python_api.py -v --color=yes
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    
    # Make sure Python module is built before running Python tests
    set_tests_properties(Python_Tests_Sequential PROPERTIES
        DEPENDS _mpi_core
    )
endif()
