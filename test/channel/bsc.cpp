#include <gtest/gtest.h>
#include "../../src/channel/BSC.h"

TEST(BSC, likelihood) {
    BSC bsc(0.1);
    ASSERT_DOUBLE_EQ(0.9, bsc.w(0, 0));
    ASSERT_DOUBLE_EQ(0.1, bsc.w(0, 1));
    ASSERT_DOUBLE_EQ(0.1, bsc.w(1, 0));
    ASSERT_DOUBLE_EQ(0.9, bsc.w(1, 1));
}

TEST(BSC, combine) {
    BSC bsc(0.1);
    const std::vector<int> expected1 = {1, 1, 1, 1};
    ASSERT_EQ(bsc.combine({0, 0, 0, 1}), expected1);

    const std::vector<int> expected2 = {0, 1, 1, 0};
    ASSERT_EQ(bsc.combine({0, 1, 1, 0}), expected2);

    const std::vector<int> expected3 = {0, 1, 0, 1, 0, 1, 1, 0};
    ASSERT_EQ(bsc.combine({0, 0, 0, 1, 1, 1, 1, 0}), expected3);
}
