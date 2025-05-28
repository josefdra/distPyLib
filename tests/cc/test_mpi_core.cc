#include <gtest/gtest.h>
#include "mpi_wrapper.hh"
#include <vector>

class MPITest : public ::testing::Test {
protected:
    void SetUp() override {
        wrapper = std::make_unique<MPIWrapper>();
    }
    
    std::unique_ptr<MPIWrapper> wrapper;
};

TEST_F(MPITest, InitializationTest) {
    EXPECT_TRUE(wrapper->is_initialized());
    EXPECT_GE(wrapper->get_size(), 1);
    EXPECT_GE(wrapper->get_rank(), 0);
    EXPECT_LT(wrapper->get_rank(), wrapper->get_size());
}

TEST_F(MPITest, ParallelComputeTest) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto results = wrapper->parallel_compute(data);
    
    EXPECT_EQ(results.size(), wrapper->get_size());
    
    // Sum should be roughly the same across processes
    double total = 0.0;
    for (double val : results) {
        total += val;
    }
    EXPECT_GT(total, 0.0);
}
