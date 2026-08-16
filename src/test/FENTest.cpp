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

  /*
  IMPORTING TESTS
  */

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

  /*
  EXPORTING TESTS
  */

  TEST_F(FenTest, ExportStandardStart) {
    std::string expected_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    ASSERT_TRUE(IO::Fen::load(board, expected_fen));
    EXPECT_EQ(IO::Fen::export_fen(board), expected_fen);
  }

  TEST_F(FenTest, ExportEmptyBoard) {
    board.clear(); 
    // ensure counters are in standard base state, incase someone wrecks board.clear up (me)
    board.set_halfmove_clock(0);
    board.set_fullmove_counter(1);

    std::string expected_fen = "8/8/8/8/8/8/8/8 w - - 0 1";
    EXPECT_EQ(IO::Fen::export_fen(board), expected_fen);
  }

  TEST_F(FenTest, ExportRoundTripTrickyPositions) {
    // collection of interesting positions
    std::vector<std::string> tricky_fens = {
      // Kiwipete
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
      
      // many empty squares, no castling
      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
      
      // tests black to move, en passant and partial castling
      "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R b KQ e3 1 8"
    };

    for (const auto& fen : tricky_fens) {
      ASSERT_TRUE(IO::Fen::load(board, fen));
      EXPECT_EQ(IO::Fen::export_fen(board), fen);
    }
  }

} // namespace Engine