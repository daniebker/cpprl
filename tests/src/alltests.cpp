#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Addition, PositiveNumbers) {
  EXPECT_EQ(2, 1 + 1);
  EXPECT_EQ(3, 1 + 2);
  EXPECT_EQ(4, 2 + 2);
}
