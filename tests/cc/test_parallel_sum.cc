#include <gtest/gtest.h>
#include "parallel_sum.hh"
#include <vector>

class MPITest : public ::testing::Test {
protected:
    void SetUp() override {
        paraSum = std::make_unique<ParaSum>();
    }
    
    std::unique_ptr<ParaSum> paraSum;
};

TEST_F(MPITest, InitializationTest) {
    EXPECT_TRUE(paraSum->is_initialized());
    EXPECT_GE(paraSum->get_size(), 1);
    EXPECT_GE(paraSum->get_rank(), 0);
    EXPECT_LT(paraSum->get_rank(), paraSum->get_size());
}

TEST_F(MPITest, ParallelSumTest) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto result = paraSum->parallel_sum(data);
    
    double total = 0.0;
    for (double val : data) {
        total += val;
    }
    EXPECT_GT(total, result);
}
