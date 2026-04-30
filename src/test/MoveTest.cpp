#include <gtest/gtest.h>

#include "Move.hpp"

#include <algorithm>
#include <vector>

namespace Engine {

  // verify default constructor creates a null move
  TEST(MoveTest, DefaultConstructorIsNull) {
    Move m;
    EXPECT_TRUE(m.is_null());
    EXPECT_EQ(m.get_raw(), 0);
  }

  // verify standard packing and unpacking for various squares/flags
  TEST(MoveTest, PackingAndUnpacking) {
    // Arbitrary middle-board move
    Move m1{12, 34, MoveFlag::CAPTURE};
    EXPECT_EQ(m1.from(), 12);
    EXPECT_EQ(m1.to(), 34);
    EXPECT_EQ(m1.flag(), MoveFlag::CAPTURE);
    EXPECT_FALSE(m1.is_null());

    // boundary squares 0 and 63
    Move m2{0, 63, MoveFlag::PC_QUEEN};
    EXPECT_EQ(m2.from(), 0);
    EXPECT_EQ(m2.to(), 63);
    EXPECT_EQ(m2.flag(), MoveFlag::PC_QUEEN);
    EXPECT_FALSE(m2.is_null());
  }

  // ensure fields don't affect each other
  TEST(MoveTest, BitIsolation) {
    // maximum possible value for "from" to see if it affects "to"
    Move m1{63, 0, MoveFlag::QUIET};
    EXPECT_EQ(m1.from(), 63);
    EXPECT_EQ(m1.to(), 0);
    EXPECT_EQ(m1.flag(), MoveFlag::QUIET);

    // maximum possible value for "to" to see if it affects "flags" 
    Move m2{0, 63, MoveFlag::QUIET};
    EXPECT_EQ(m2.from(), 0);
    EXPECT_EQ(m2.to(), 63);
    EXPECT_EQ(m2.flag(), MoveFlag::QUIET);
  }

  // equality operators
  TEST(MoveTest, EqualityOperators) {
    Move a{10, 20, MoveFlag::QUIET};
    Move b{10, 20, MoveFlag::QUIET};
    Move c{10, 21, MoveFlag::QUIET};
    Move d{11, 20, MoveFlag::QUIET};
    Move e{10, 20, MoveFlag::CAPTURE};

    EXPECT_EQ(a,b);
    EXPECT_NE(a,c);
    EXPECT_NE(a,d);
    EXPECT_NE(a,e);
  }

  // priority sorting logic
  // verifies that "thousands" flag placing logic works for move ordering
  TEST(MoveTest, NaturalOrdering) {
    Move quiet{10, 20, MoveFlag::QUIET};
    Move doublePush{10, 20, MoveFlag::DOUBLE_PAWN_PUSH};
    Move capture{10, 20, MoveFlag::CAPTURE};
    Move promotion{10, 20, MoveFlag::P_QUEEN};
    Move promotionCapture{10, 20, MoveFlag::PC_QUEEN};

    // assert that higher-value flags result in "better move" values
    EXPECT_LT(quiet, capture);
    EXPECT_LT(capture, promotion);
    EXPECT_LT(promotion, promotionCapture);
    EXPECT_GT(promotionCapture, quiet);

    // check specific bit hierarchy (quiet (0) < double push (1) < castle (2/3) < capture (4))
    EXPECT_LT(quiet, doublePush);
    EXPECT_LT(doublePush, capture);
  }

  TEST(MoveTest, IntegrationWithStdSort) {
    std::vector<Move> moves = {
      Move(1, 2, MoveFlag::QUIET),
      Move(3, 4, MoveFlag::PC_QUEEN), // highest priority
      Move(5, 6, MoveFlag::CAPTURE),
      Move(7, 8, MoveFlag::P_KNIGHT)
    };

    // sort in descending order
    std::sort(moves.begin(), moves.end(), std::greater<Move>());

    EXPECT_EQ(moves[0].flag(), MoveFlag::PC_QUEEN);
    EXPECT_EQ(moves[1].flag(), MoveFlag::P_KNIGHT);
    EXPECT_EQ(moves[2].flag(), MoveFlag::CAPTURE);
    EXPECT_EQ(moves[3].flag(), MoveFlag::QUIET);
  }
  
}