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
add_library(parallel_sum STATIC
    src/cc/parallel_sum.cc
)

target_link_libraries(parallel_sum 
    MPI::MPI_CXX
)

target_include_directories(parallel_sum PUBLIC
    ${CMAKE_SOURCE_DIR}/src/cc
    ${MPI_CXX_INCLUDE_DIRS}
)

# Create MPI worker executable
add_executable(mpi_worker
    src/cc/mpi_worker.cc
)

target_link_libraries(mpi_worker
    MPI::MPI_CXX
)

# Create Python module
pybind11_add_module(_parallel_sum 
    src/cc/python_bindings.cc
)

target_link_libraries(_parallel_sum PRIVATE 
    parallel_sum
    MPI::MPI_CXX
)

# Compiler-specific options
target_compile_definitions(_parallel_sum PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

# Install Python module and executable
install(TARGETS _parallel_sum DESTINATION distPyLib)
install(TARGETS mpi_worker 
        RUNTIME DESTINATION distPyLib
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
                   GROUP_READ GROUP_EXECUTE 
                   WORLD_READ WORLD_EXECUTE)

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
    
    # Make sure Python module and worker are built before running Python tests
    set_tests_properties(Python_Tests_Sequential PROPERTIES
        DEPENDS "_parallel_sum;mpi_worker"
    )
endif()
