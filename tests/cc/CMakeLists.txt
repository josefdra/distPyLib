# Find GTest
find_package(GTest REQUIRED)

# Create test executable
add_executable(test_runner
    test_main.cc
    test_mpi_core.cc
)

target_link_libraries(test_runner
    mpi_core
    GTest::gtest
    GTest::gtest_main
    MPI::MPI_CXX
)

# Add test
add_test(NAME MPI_Tests COMMAND mpirun -n 2 ./test_runner)
