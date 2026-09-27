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

    // initialize move generation before tests
    static void SetUpTestSuite(void) {
      MoveGenerator::init_tables();
    }

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
    auto moves = MoveGenerator::pseudo_legal_moves(board);
    
    EXPECT_EQ(moves.size(), 20);
    
    // check specific pawn and knight moves
    EXPECT_TRUE(move_exists(moves, Square::SQ_E2, Square::SQ_E4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_G1, Square::SQ_F3));
    
    // check illegal move
    EXPECT_FALSE(move_exists(moves, Square::SQ_E2, Square::SQ_E5));
  }

  TEST_F(MoveGeneratorTest, AbsolutePins) {
    IO::Fen::load(board, "4q3/8/8/8/8/8/4R3/4K3 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_E2, Square::SQ_E5)); // can stay on file
    EXPECT_FALSE(move_exists(moves, Square::SQ_E2, Square::SQ_D2)); // cannot leave file
  }

  TEST_F(MoveGeneratorTest, EnPassantCapture) {
    IO::Fen::load(board, "rnbqkbnr/pp1ppppp/8/2pP4/8/8/PPP1PPPP/RNBQKBNR w KQkq c6 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_C6)); 
  }

  TEST_F(MoveGeneratorTest, DoubleCheckResponse) {
    IO::Fen::load(board, "4r3/8/8/8/8/5n2/8/4K3 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    for (const auto& move : moves) {
      EXPECT_EQ(move.from(), Square::SQ_E1);
    }
  }

  TEST_F(MoveGeneratorTest, CastlingLegality) {
    IO::Fen::load(board, "r3k2r/8/8/8/8/8/8/R1B1K2R w K - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_C1));

    IO::Fen::load(board, "r3k2r/8/8/3q4/8/8/8/R3K2R w KQkq - 0 1");
    moves = MoveGenerator::pseudo_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_G1)); 
  }

  TEST_F(MoveGeneratorTest, PawnPromotion) {
    IO::Fen::load(board, "8/P7/8/8/8/8/8/k1K5 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

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
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_FALSE(move_exists(moves, Square::SQ_D5, Square::SQ_C6));
  }

  TEST_F(MoveGeneratorTest, CastlingRestrictions) {
    // cannot castle OUT OF check
    IO::Fen::load(board, "r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_G1));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_C1));

    // cannot castle THROUGH check
    IO::Fen::load(board, "r3k2r/8/8/5q2/8/8/8/R3K2R w KQkq - 0 1");
    moves = MoveGenerator::pseudo_legal_moves(board);
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_G1));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E1, Square::SQ_C1));
  }

  TEST_F(MoveGeneratorTest, CheckEvasion) {
    // White King on e1, Black Queen on e8 (Check!). 
    // White has a Bishop on d2 that can block on e3.
    IO::Fen::load(board, "4q3/8/8/8/8/8/3B4/4K3 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_D2, Square::SQ_E3)); // Block
    EXPECT_TRUE(move_exists(moves, Square::SQ_E1, Square::SQ_D1)); // King move
    
    // Any move that doesn't resolve check should be missing
    EXPECT_FALSE(move_exists(moves, Square::SQ_D2, Square::SQ_C3)); 
  }

  TEST_F(MoveGeneratorTest, KingSafety) {
    IO::Fen::load(board, "3r4/8/8/8/8/8/8/4K3 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_D1)); // stepping into rook's fire
    EXPECT_FALSE(move_exists(moves, Square::SQ_E1, Square::SQ_D2)); // stepping into rook's fire
    EXPECT_TRUE(move_exists(moves, Square::SQ_E1, Square::SQ_F1));  // safe square
  }

  TEST_F(MoveGeneratorTest, Stalemate) {
    IO::Fen::load(board, "k7/2Q5/2K5/8/8/8/8/8 b - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_EQ(moves.size(), 0);
  }

  TEST_F(MoveGeneratorTest, BlackPawnPushes) {
    IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);
    
    // single and double pushes
    EXPECT_TRUE(move_exists(moves, Square::SQ_E7, Square::SQ_E6));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E7, Square::SQ_E5));
  }

  TEST_F(MoveGeneratorTest, BlackEnPassantCapture) {
    // white just played e2 to e4, black has a pawn on d4
    IO::Fen::load(board, "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    // black pawn on d4 to en passant on e3
    EXPECT_TRUE(move_exists(moves, Square::SQ_D4, Square::SQ_E3)); 
  }

  TEST_F(MoveGeneratorTest, BlackPawnPromotion) {
    // black pawn on a2 promotion on a1
    IO::Fen::load(board, "K1k5/8/8/8/8/8/p7/8 b - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    unsigned int promotions = 0;
    for (const auto& move : moves) {
      if (move.to() == Square::SQ_A1) promotions++;
    }
    EXPECT_EQ(promotions, 4); 
  }

  TEST_F(MoveGeneratorTest, KnightCenterMobility) {
    // white knight on e4
    IO::Fen::load(board, "k7/8/8/8/4N3/8/8/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    // should have 8 valid moves
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D6));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F6));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_C5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_G5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_C3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_G3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D2));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F2));

    size_t knight_move_count = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_E4) knight_move_count++;
    }
    EXPECT_EQ(knight_move_count, 8);
  }

  TEST_F(MoveGeneratorTest, KnightCornerAndEdgeWrapAround) {
    // white Knight on a1
    IO::Fen::load(board, "k7/8/8/8/8/8/8/N3K3 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    // only 2 valid moves from a1
    EXPECT_TRUE(move_exists(moves, Square::SQ_A1, Square::SQ_B3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_A1, Square::SQ_C2));

    // no wrap around moves to H-file or G-file occurred?
    EXPECT_FALSE(move_exists(moves, Square::SQ_A1, Square::SQ_H2));
    EXPECT_FALSE(move_exists(moves, Square::SQ_A1, Square::SQ_G2));

    size_t knight_move_count = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_A1) knight_move_count++;
    }
    EXPECT_EQ(knight_move_count, 2);
  }

  TEST_F(MoveGeneratorTest, KnightCapturesAndFriendlyBlockade) {
    // white knight on e4
    // friendly pawns blocking c3, g3, d2, f2
    // hostile pawns on d6, f6 (captures)
    // empty squares on c5, g5
    IO::Fen::load(board, "k7/8/3p1p2/8/4N3/2P3P1/3P1P2/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D6));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F6));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_C5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_G5));

    // blocked by friendly pieces (should NOT exist)
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_C3));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_G3));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_D2));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_F2));
  }

  TEST_F(MoveGeneratorTest, BlackKnightMoves) {
    // black turn, black Knight on d5
    IO::Fen::load(board, "k7/8/8/3n4/8/8/8/K7 b - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_C7));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_E7));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_B6));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_F6));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_B4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_F4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_C3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_D5, Square::SQ_E3));
  }

  TEST_F(MoveGeneratorTest, BishopCenterMobility) {
    IO::Fen::load(board, "k7/8/8/8/4B3/8/8/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    size_t bishop_move_count = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_E4) bishop_move_count++;
    }
    EXPECT_EQ(bishop_move_count, 13);

    // check extremes to make sure it reaches the edges correctly
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_A8));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_H7));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_H1));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_B1));
  }

  TEST_F(MoveGeneratorTest, BishopCapturesAndBlockades) {
    IO::Fen::load(board, "k7/8/2p3P1/8/4B3/8/2P3p1/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F5));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_G6));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_H7));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_C6));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_B7));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D3));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_C2));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_G2));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_H1));
  }

  TEST_F(MoveGeneratorTest, RookCenterMobility) {
    IO::Fen::load(board, "k7/8/8/8/4R3/8/8/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    size_t rook_move_count = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_E4) rook_move_count++;
    }
    EXPECT_EQ(rook_move_count, 14);

    // check extremes
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E8));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E1));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_A4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_H4));
  }

  TEST_F(MoveGeneratorTest, RookCapturesAndBlockades) {
    IO::Fen::load(board, "k7/4p3/8/8/1P2R1p1/8/4P3/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E7));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_E8));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E3));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_E2));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_G4));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_H4));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_C4));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_B4));
  }

  TEST_F(MoveGeneratorTest, QueenCenterMobility) {
    IO::Fen::load(board, "k7/8/8/8/4Q3/8/8/K7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    size_t queen_move_count = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_E4) queen_move_count++;
    }
    EXPECT_EQ(queen_move_count, 27);
  }

  TEST_F(MoveGeneratorTest, KingCenterMobility) {
    IO::Fen::load(board, "8/8/8/8/4K3/8/8/k7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F3));

    size_t king_moves = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_E4) king_moves++;
    }
    EXPECT_EQ(king_moves, 8);
  }

  TEST_F(MoveGeneratorTest, KingEdgeAndCornerMobility) {
    IO::Fen::load(board, "K7/8/8/8/8/8/8/k7 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_TRUE(move_exists(moves, Square::SQ_A8, Square::SQ_B8));
    EXPECT_TRUE(move_exists(moves, Square::SQ_A8, Square::SQ_A7));
    EXPECT_TRUE(move_exists(moves, Square::SQ_A8, Square::SQ_B7));

    size_t king_moves = 0;
    for (const auto& move : moves) {
      if (move.from() == Square::SQ_A8) king_moves++;
    }
    EXPECT_EQ(king_moves, 3);
  }

  TEST_F(MoveGeneratorTest, KingCapturesAndBlockades) {
    IO::Fen::load(board, "k7/8/8/4P3/3PKp2/5p2/8/8 w - - 0 1");
    auto moves = MoveGenerator::pseudo_legal_moves(board);

    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_E5));
    EXPECT_FALSE(move_exists(moves, Square::SQ_E4, Square::SQ_D4));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F4));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F3));

    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_F5));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_D3));
    EXPECT_TRUE(move_exists(moves, Square::SQ_E4, Square::SQ_E3));
  }

} // namespace Engine