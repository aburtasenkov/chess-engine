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
  TEST_F(BoardTest, InitialCorrectnesss) {
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

    EXPECT_EQ(board.get_side_to_move(), Color::WHITE);
    EXPECT_EQ(board.get_castling_rights(), CastlingRights::ALL_CASTLING);
    EXPECT_EQ(board.get_en_passant_target(), Square::SQ_NONE);
  }

  // ensure no pieces overlap on default construction
  TEST_F(BoardTest, NoColorOverlap) {
    uint64_t white_bitboard = board.get_color_bitboard(WHITE);
    uint64_t black_bitboard = board.get_color_bitboard(BLACK);

    EXPECT_EQ(white_bitboard & black_bitboard, 0ULL) << "White and Black pieces are overlapping";
  }

  // verify the total bitboard is the union of black and white pieces
  TEST_F(BoardTest, TotalOccupancyIntegrity) {
    uint64_t total_bitboard = board.get_color_bitboard(WHITE) | board.get_color_bitboard(BLACK);

    EXPECT_EQ(board.get_total_bitboard(), total_bitboard);
  }

  TEST_F(BoardTest, AllBitBoardCheck) {
    // catches a previous initialization bug
    EXPECT_NE(board.get_piece_bitboard(WHITE, ALL), 0UL);
  }

  TEST_F(BoardTest, SetSinglePiece) {
    board.set_piece(SQ_E4, Color::WHITE, PieceType::KNIGHT);

    EXPECT_TRUE(is_bit_set(board.get_piece_bitboard(Color::WHITE, PieceType::KNIGHT), SQ_E4));
    EXPECT_TRUE(is_bit_set(board.get_color_bitboard(Color::WHITE), SQ_E4));
    EXPECT_TRUE(is_bit_set(board.get_total_bitboard(), SQ_E4));
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

  TEST_F(BoardTest, SetSideToMove) {
    board.set_side_to_move(Color::BLACK);
    EXPECT_EQ(board.get_side_to_move(), Color::BLACK);

    board.set_side_to_move(Color::WHITE);
    EXPECT_EQ(board.get_side_to_move(), Color::WHITE);
  }

  TEST_F(BoardTest, CastlingRightsLogic) {
    board.set_castling_rights(static_cast<CastlingRights>(BLACK_OO | BLACK_OOO));
    
    EXPECT_FALSE(board.get_castling_rights() & WHITE_OO);
    EXPECT_FALSE(board.get_castling_rights() & WHITE_OOO);
    EXPECT_TRUE(board.get_castling_rights() & BLACK_OO);
  }

  TEST_F(BoardTest, EnPassantTarget) {
    board.set_en_passant_target(Square::SQ_D3);
    EXPECT_EQ(board.get_en_passant_target(), Square::SQ_D3);

    board.set_en_passant_target(Square::SQ_NONE);
    EXPECT_EQ(board.get_en_passant_target(), Square::SQ_NONE);
  }
}