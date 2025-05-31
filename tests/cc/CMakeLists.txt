# Find GTest
find_package(GTest REQUIRED)

# Create test executable
add_executable(test_runner
    test_main.cc
)

target_link_libraries(test_runner
    GTest::gtest
    GTest::gtest_main
    MPI::MPI_CXX
)
