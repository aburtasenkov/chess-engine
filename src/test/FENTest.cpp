#include <gtest/gtest.h>

#include "FEN.hpp"
#include "Board.hpp"

namespace Engine {

  class FenTest : public ::testing::Test {
  protected:
    Board board;

    // helper to check if a square has a specific piece
    bool has_piece(Color color, PieceType piece, Square square) {
      return (board.get_piece_bitboard(color, piece) & (1ULL << square)) != 0;
    }
  };

  TEST_F(FenTest, ParseStandardStart) {
    std::string_view fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    ASSERT_TRUE(IO::Fen::load(board, fen));

    EXPECT_EQ(board.get_side_to_move(), Color::WHITE);
    EXPECT_EQ(board.get_castling_rights(), CastlingRights::ALL_CASTLING);
    EXPECT_EQ(board.get_en_passant_target(), Square::SQ_NONE);
    
    // verify a few key pieces
    EXPECT_TRUE(has_piece(Color::WHITE, PieceType::ROOK, Square::SQ_A1));
    EXPECT_TRUE(has_piece(Color::BLACK, PieceType::KING, Square::SQ_E8));
    EXPECT_TRUE(has_piece(Color::WHITE, PieceType::PAWN, Square::SQ_C2));
  }

  TEST_F(FenTest, ParseSicilianDefense) {
    // 1. e4 c5 2. Nf3
    std::string_view fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
    ASSERT_TRUE(IO::Fen::load(board, fen));

    EXPECT_EQ(board.get_side_to_move(), Color::BLACK);
    
    EXPECT_TRUE(has_piece(Color::BLACK, PieceType::PAWN, Square::SQ_C5));
    EXPECT_TRUE(has_piece(Color::WHITE, PieceType::PAWN, Square::SQ_E4));
    EXPECT_TRUE(has_piece(Color::WHITE, PieceType::KNIGHT, Square::SQ_F3));
    
    // Ensure old squares are empty
    EXPECT_FALSE(has_piece(Color::WHITE, PieceType::PAWN, Square::SQ_E2));
  }

  TEST_F(FenTest, EnPassantValidation) {
    // validate white en passant target (Rank 3)
    EXPECT_TRUE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"));
    EXPECT_EQ(board.get_en_passant_target(), Square::SQ_E3);

    // validate black en passant target (Rank 6)
    EXPECT_TRUE(IO::Fen::load(board, "rnbqkbnr/pppp1ppp/8/4p3/8/8/PPPPPPPP/RNBQKBNR w KQkq e6 0 1"));
    EXPECT_EQ(board.get_en_passant_target(), Square::SQ_E6);

    // invalid en passant target (Rank 4 - not possible in chess)
    EXPECT_FALSE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e4 0 1"));
  }

  TEST_F(FenTest, PartialCastlingRights) {
    // only white O-O and Black O-O-O
    EXPECT_TRUE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kq - 0 1"));
    EXPECT_EQ(board.get_castling_rights(), (CastlingRights::WHITE_OO | CastlingRights::BLACK_OOO));

    // no castling
    EXPECT_TRUE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1"));
    EXPECT_EQ(board.get_castling_rights(), CastlingRights::NO_CASTLING);
  }

  TEST_F(FenTest, MalformedInputHandling) {
    // too few segments
    EXPECT_FALSE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w"));

    // invalid characters
    EXPECT_FALSE(IO::Fen::load(board, "rnbqkbnr/ppXppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

    // invalid side to move
    EXPECT_FALSE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1"));

    // too many files
    EXPECT_FALSE(IO::Fen::load(board, "rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
    
    // invalid castling char
    EXPECT_FALSE(IO::Fen::load(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w K!kq - 0 1"));
  }

} // namespace Engine