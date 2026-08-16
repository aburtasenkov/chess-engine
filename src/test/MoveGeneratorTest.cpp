#include <gtest/gtest.h>

#include "MoveGenerator.hpp"
#include "Board.hpp"
#include "MoveList.hpp"
#include "Move.hpp"
#include "FEN.hpp"
#include "Constants.hpp"

namespace Engine {

  class MoveGeneratorTest : public ::testing::Test {
  protected:
    Board board;

    // helper to find a move by its coordinates
    bool move_exists(const MoveList& list, Square from, Square to) {
      for (const auto& move : list) {
        if (move.from() == from && move.to() == to) {
          return true;
        }
      }
      return false;
    }
  };

  TEST_F(MoveGeneratorTest, InitialPosition) {
    IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);
    
    EXPECT_EQ(moves.size(), 20);
    
    // check specific pawn and knight moves
    EXPECT_TRUE(move_exists(moves, Square::SQ_E2, Square::SQ_E4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_G1, Square::SQ_F3));
    
    // check illegal move
    EXPECT_FALSE(move_exists(moves, Square::SQ_E2, Square::SQ_E5));
  }

  TEST_F(MoveGeneratorTest, AbsolutePins) {
    IO::Fen::load(board, "4q3/8/8/8/8/8/4R3/4K3 w - - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_E2, Square::SQ_E5)); // can stay on file
    EXPECT_FALSE(move_exists(moves, Square::SQ_E2, Square::SQ_D2)); // cannot leave file
  }

  TEST_F(MoveGeneratorTest, EnPassantCapture) {
    IO::Fen::load(board, "rnbqkbnr/pp1ppppp/8/2pP4/8/8/PPP1PPPP/RNBQKBNR w KQkq c6 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_C6)); 
  }

  TEST_F(MoveGeneratorTest, DoubleCheckResponse) {
    IO::Fen::load(board, "4r3/8/8/8/8/5n2/8/4K3 w - - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    for (const auto& move : moves) {
      EXPECT_EQ(move.from(), Square::SQ_E1);
    }
  }

  TEST_F(MoveGeneratorTest, CastlingLegality) {
    IO::Fen::load(board, "r3k2r/8/8/8/8/8/8/R1B1K2R w K - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_C1));

    IO::Fen::load(board, "r3k2r/8/8/3q4/8/8/8/R3K2R w KQkq - 0 1");
    moves = MoveGenerator::generate_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_G1)); 
  }

  TEST_F(MoveGeneratorTest, PawnPromotion) {
    IO::Fen::load(board, "8/P7/8/8/8/8/8/k1K5 w - - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    // should have 4 promotion moves + some king moves
    // specifically check for the 4 promotions on a8
    unsigned int promotions = 0;
    for (const auto& move : moves) {
      if (move.to() == Square::SQ_A8) promotions++;
    }
    EXPECT_EQ(promotions, 4); 
  }

  TEST_F(MoveGeneratorTest, EnPassantDiscoveredCheck) {
    IO::Fen::load(board, "8/8/8/k1pP3R/8/8/8/4K3 w - c6 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    EXPECT_FALSE(move_exists(moves, Square::SQ_D5, Square::SQ_C6));
  }

  TEST_F(MoveGeneratorTest, CastlingRestrictions) {
    // cannot castle OUT OF check
    IO::Fen::load(board, "r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_G1));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_C1));

    // cannot castle THROUGH check
    IO::Fen::load(board, "r3k2r/8/8/5q2/8/8/8/R3K2R w KQkq - 0 1");
    moves = MoveGenerator::generate_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_G1));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E1, Square::SQ_C1));
  }

  TEST_F(MoveGeneratorTest, CheckEvasion) {
    // White King on e1, Black Queen on e8 (Check!). 
    // White has a Bishop on d2 that can block on e3.
    IO::Fen::load(board, "4q3/8/8/8/8/8/3B4/4K3 w - - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_D2, Square::SQ_E3)); // Block
    EXPECT_TRUE(move_exists(moves, Square::SQ_E1, Square::SQ_D1)); // King move
    
    // Any move that doesn't resolve check should be missing
    EXPECT_FALSE(move_exists(moves, Square::SQ_D2, Square::SQ_C3)); 
  }

  TEST_F(MoveGeneratorTest, KingSafety) {
    IO::Fen::load(board, "3r4/8/8/8/8/8/8/4K3 w - - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_D1)); // stepping into rook's fire
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_D2)); // stepping into rook's fire
    EXPECT_TRUE(move_exists(moves, Square::SQ_E1, SQ_F1));  // safe square
  }

  TEST_F(MoveGeneratorTest, Stalemate) {
    IO::Fen::load(board, "k7/2Q5/2K5/8/8/8/8/8 b - - 0 1");
    auto moves = MoveGenerator::generate_legal_moves(board);

    EXPECT_EQ(moves.size(), 0);
  }

} // namespace Engine