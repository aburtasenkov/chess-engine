#include <gtest/gtest.h>

int Addition(int a, int b) {
  return a + b;
}

TEST(MathLogic, AdditionWorks) {
  EXPECT_EQ(Addition(3, 3), 6);
  EXPECT_EQ(Addition(1, 2), 3);
}