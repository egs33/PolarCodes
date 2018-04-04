#include <gtest/gtest.h>
#include "../../src/channel/BEC.h"

TEST(BEC, likelihood) {
    BEC bec(0.1);
    ASSERT_DOUBLE_EQ(0.9, bec.w(0, 0));
    ASSERT_DOUBLE_EQ(0, bec.w(0, 1));
    ASSERT_DOUBLE_EQ(0, bec.w(1, 0));
    ASSERT_DOUBLE_EQ(0.9, bec.w(1, 1));
    ASSERT_DOUBLE_EQ(0.1, bec.w(2, 0));
    ASSERT_DOUBLE_EQ(0.1, bec.w(2, 1));
}

TEST(BEC, combine) {
    BEC bec(0.1);
    const std::vector<int> expected1 = {1, 1, 1, 1};
    ASSERT_EQ(bec.combine({0, 0, 0, 1}), expected1);

    const std::vector<int> expected2 = {0, 1, 1, 0};
    ASSERT_EQ(bec.combine({0, 1, 1, 0}), expected2);

    const std::vector<int> expected3 = {0, 1, 0, 1, 0, 1, 1, 0};
    ASSERT_EQ(bec.combine({0, 0, 0, 1, 1, 1, 1, 0}), expected3);
}

TEST(BEC, symmetricCapacity) {
    BEC bec1(0.1);
    ASSERT_EQ(bec1.symmetricCapacity(1, 0), 0.9);
    ASSERT_EQ(bec1.symmetricCapacity(2, 0), 0.81);
    ASSERT_EQ(bec1.symmetricCapacity(2, 1), 0.99);

    BEC bec2(0.5);
    ASSERT_EQ(bec2.symmetricCapacity(1, 0), 0.5);
    ASSERT_EQ(bec2.symmetricCapacity(2, 0), 0.25);
    ASSERT_EQ(bec2.symmetricCapacity(2, 1), 0.75);
}

