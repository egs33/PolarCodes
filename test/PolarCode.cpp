#include <gtest/gtest.h>
#include "channel/BEC.h"
#include "PolarCode.h"

TEST(encode, length_4) {
    BEC bec(0.5);
    PolarCode code(4, 2, bec);
    const std::vector<int> expected1 = {0, 0, 0, 0};
    ASSERT_EQ(code.encode({0, 0}), expected1);

    const std::vector<int> expected2 = {1, 1, 1, 1};
    ASSERT_EQ(code.encode({0, 1}), expected2);
}

TEST(encode, length_64) {
    BEC bec(0.1);
    PolarCode code(64, 50, bec);
    const std::vector<int> expected1 =
            {1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,};
    ASSERT_EQ(code.encode(
        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1}
        ), expected1);

    const std::vector<int> expected2 =
            {1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1};
    ASSERT_EQ(code.encode(
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        ), expected2);
}


