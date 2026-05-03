#include <gtest/gtest.h>

#include "MoveList.hpp"

namespace Engine {
  // default initialization
  TEST(MoveListTest, DefaultConstructorIsNull) {
    MoveList list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.begin(), list.end());
  }

  // self-explanatory
  TEST(MoveListTest, PushAndAccess) {
    MoveList list;

    Move m1{};
    Move m2{};

    list.push_back(m1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list[0], m1);

    list.push_back(m2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list[1], m2);
  }

  // verify iterator support
  TEST(MoveListTest, IteratorLoop) {
    MoveList list;
    list.push_back(Move(1, 2, MoveFlag::QUIET));
    list.push_back(Move(3, 4, MoveFlag::QUIET));
    list.push_back(Move(5, 6, MoveFlag::QUIET));

    int count = 0;
    for (const auto& m: list) {
      (void)m; // suppress unused variable warning
      count++;
    }

    EXPECT_EQ(count, 3);
  }

  // clear method functionality
  TEST(MoveListTest, ClearList) {
    MoveList list;
    list.push_back(Move(1, 2, MoveFlag::QUIET));
    list.push_back(Move(3, 4, MoveFlag::QUIET));

    EXPECT_EQ(list.size(), 2);

    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.begin(), list.end());
  }

  // const correctness
  TEST(MoveListTest, ConstAccess) {
    MoveList list;
    list.push_back(Move(10, 10, MoveFlag::CAPTURE));

    const MoveList& constList = list;

    EXPECT_EQ(constList[0].flag(), MoveFlag::CAPTURE);
    EXPECT_EQ(constList.begin()->from(), 10);
  }
}