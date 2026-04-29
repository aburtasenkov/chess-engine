#include <gtest/gtest.h>

#include "Board.hpp"

namespace Engine {

  class BoardTest : public ::testing::Test {
  protected:
    Board board;
  };

  // starting bitboard must match the constants
  TEST_F(BoardTest, StartingPositionCorrectness) {
    EXPECT_EQ(board.get_piece_bitboard(WHITE, PAWN), StartPos::WhitePawns);
    EXPECT_EQ(board.get_piece_bitboard(WHITE, KNIGHT), StartPos::WhiteKnights);
    EXPECT_EQ(board.get_piece_bitboard(WHITE, BISHOP), StartPos::WhiteBishops);
    EXPECT_EQ(board.get_piece_bitboard(WHITE, ROOK), StartPos::WhiteRooks);
    EXPECT_EQ(board.get_piece_bitboard(WHITE, QUEEN), StartPos::WhiteQueen);
    EXPECT_EQ(board.get_piece_bitboard(WHITE, KING), StartPos::WhiteKing);
    EXPECT_EQ(board.get_piece_bitboard(WHITE, ALL), StartPos::WhiteAll);

    EXPECT_EQ(board.get_piece_bitboard(BLACK, PAWN), StartPos::BlackPawns);
    EXPECT_EQ(board.get_piece_bitboard(BLACK, KNIGHT), StartPos::BlackKnights);
    EXPECT_EQ(board.get_piece_bitboard(BLACK, BISHOP), StartPos::BlackBishops);
    EXPECT_EQ(board.get_piece_bitboard(BLACK, ROOK), StartPos::BlackRooks);
    EXPECT_EQ(board.get_piece_bitboard(BLACK, QUEEN), StartPos::BlackQueen);
    EXPECT_EQ(board.get_piece_bitboard(BLACK, KING), StartPos::BlackKing);
    EXPECT_EQ(board.get_piece_bitboard(BLACK, ALL), StartPos::BlackAll);
  }

  // ensure no pieces overlap on default construction
  TEST_F(BoardTest, NoColorOverlap) {
    uint64_t white_occupancy = board.get_color_occupancy(WHITE);
    uint64_t black_occupancy = board.get_color_occupancy(BLACK);

    EXPECT_EQ(white_occupancy & black_occupancy, 0ULL) << "White and Black pieces are overlapping";
  }

  // verify the total occupancy is the union of black and white pieces
  TEST_F(BoardTest, TotalOccupancyIntegrity) {
    uint64_t total_occupancy = board.get_color_occupancy(WHITE) | board.get_color_occupancy(BLACK);

    EXPECT_EQ(board.get_total_occupancy(), total_occupancy);
  }

  TEST_F(BoardTest, AllBitBoardCheck) {
    // catches a previous initialization bug
    EXPECT_NE(board.get_piece_bitboard(WHITE, ALL), 0UL);
  }
}