#include <gtest/gtest.h>

#include "Board.hpp"

namespace Engine {

  class BoardTest : public ::testing::Test {
  protected:
    Board board;

    // helper to check if a specific square is set in a bitboard
    bool is_bit_set(uint64_t bitboard, Square square) {
      return (bitboard >> static_cast<uint8_t>(square)) & 1ULL;
    }
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

  TEST_F(BoardTest, SetSinglePiece) {
    board.set_piece(SQ_E4, Color::WHITE, PieceType::KNIGHT);

    EXPECT_TRUE(is_bit_set(board.get_piece_bitboard(Color::WHITE, PieceType::KNIGHT), SQ_E4));
    EXPECT_TRUE(is_bit_set(board.get_color_occupancy(Color::WHITE), SQ_E4));
    EXPECT_TRUE(is_bit_set(board.get_total_occupancy(), SQ_E4));
  }

  TEST_F(BoardTest, SetMultiplePieces) {
    board.set_piece(SQ_A2, Color::WHITE, PieceType::PAWN);
    board.set_piece(SQ_B2, Color::WHITE, PieceType::PAWN);

    uint64_t pawns = board.get_piece_bitboard(Color::WHITE, PieceType::PAWN);
    EXPECT_TRUE(is_bit_set(pawns, SQ_A2));
    EXPECT_TRUE(is_bit_set(pawns, SQ_B2));
    
    // ensure no other bits were set
    EXPECT_FALSE(is_bit_set(pawns, SQ_A1));
  }

  TEST_F(BoardTest, ColorIsolation) {
    // place a white queen and black queen on the same index
    // to ensure they dont leak into each others bitboards
    board.set_piece(SQ_D4, Color::WHITE, PieceType::QUEEN);
    
    EXPECT_TRUE(is_bit_set(board.get_piece_bitboard(Color::WHITE, PieceType::QUEEN), SQ_D4));
    EXPECT_FALSE(is_bit_set(board.get_piece_bitboard(Color::BLACK, PieceType::QUEEN), SQ_D4));
  }

  TEST_F(BoardTest, BoundarySquares) {
    board.set_piece(SQ_A1, Color::WHITE, PieceType::ROOK);
    board.set_piece(SQ_H8, Color::BLACK, PieceType::ROOK);

    EXPECT_TRUE(is_bit_set(board.get_piece_bitboard(Color::WHITE, PieceType::ROOK), SQ_A1));
    EXPECT_TRUE(is_bit_set(board.get_piece_bitboard(Color::BLACK, PieceType::ROOK), SQ_H8));
  }
}