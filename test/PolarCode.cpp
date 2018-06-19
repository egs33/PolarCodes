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


TEST(decode, bec_length_4) {
    BEC bec(0.5);
    PolarCode code(4, 2, bec);
    const std::vector<int> expected1 = {0, 0};
    ASSERT_EQ(code.SuccessiveCancellationDecode({0, 0, 0, 0}), expected1);
    ASSERT_EQ(code.LlrSCDecode({0, 0, 0, 0}), expected1);

    const std::vector<int> expected2 = {0, 0};
    ASSERT_EQ(code.SuccessiveCancellationDecode({0, 0, 0, 2}), expected2);
    ASSERT_EQ(code.LlrSCDecode({0, 0, 0, 2}), expected2);

    const std::vector<int> expected3 = {0, 1};
    ASSERT_EQ(code.SuccessiveCancellationDecode({2, 1, 1, 1}), expected3);
    ASSERT_EQ(code.LlrSCDecode({2, 1, 1, 1}), expected3);
}


TEST(decode, bec_length_64) {
    BEC bec(0.5);
    PolarCode code(64, 20, bec);
    const std::vector<int> expected1(20, 0);
    ASSERT_EQ(code.SuccessiveCancellationDecode({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}),expected1);
    ASSERT_EQ(code.LlrSCDecode({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}), expected1);

    const std::vector<int> expected2(20, 0);
    ASSERT_EQ(code.SuccessiveCancellationDecode({2,2,2,2,0,0,2,0,0,0,2,0,2,2,2,2,0,0,0,0,0,0,2,0,0,0,2,0,2,2,0,2,0,2,0,0,0,2,0,2,0,2,0,2,2,0,0,2,2,2,2,0,2,2,0,2,0,0,2,0,0,0,0,2}), expected2);
    ASSERT_EQ(code.LlrSCDecode({2,2,2,2,0,0,2,0,0,0,2,0,2,2,2,2,0,0,0,0,0,0,2,0,0,0,2,0,2,2,0,2,0,2,0,0,0,2,0,2,0,2,0,2,2,0,0,2,2,2,2,0,2,2,0,2,0,0,2,0,0,0,0,2}), expected2);

    const std::vector<int> expected3 = { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0 };
    ASSERT_EQ(code.SuccessiveCancellationDecode({0,2,2,0,1,2,0,2,0,2,2,0,1,2,2,2,0,2,2,2,2,0,0,1,2,0,2,2,2,1,2,0,2,0,1,2,1,0,1,2,2,0,2,2,1,0,2,0,1,0,1,0,1,2,1,2,2,2,1,0,2,2,2,2,}), expected3);
    ASSERT_EQ(code.LlrSCDecode({0,2,2,0,1,2,0,2,0,2,2,0,1,2,2,2,0,2,2,2,2,0,0,1,2,0,2,2,2,1,2,0,2,0,1,2,1,0,1,2,2,0,2,2,1,0,2,0,1,0,1,0,1,2,1,2,2,2,1,0,2,2,2,2,}), expected3);
}
