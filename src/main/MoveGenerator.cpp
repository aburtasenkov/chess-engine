#include "MoveGenerator.hpp"

#include "MoveList.hpp"

namespace Engine {

  // return index of the least significant bit that is "1" 
  static inline uint16_t lsb(uint64_t bitboard) {
    return static_cast<uint16_t>(__builtin_ctzll(bitboard));
  }

  static inline uint16_t pop_lsb(uint64_t& bitboard) {
    uint16_t i = lsb(bitboard);
    bitboard &= (bitboard - 1); // clear least significant bit, while leaving everything else untouched
    return i;
  }

  MoveList MoveGenerator::generate_legal_moves(const Board& board) {
    MoveList moves;

    // generating pseudo legal moves for now
    generate_pawn_moves(board, moves);
    generate_knight_moves(board, moves);
    generate_bishop_moves(board, moves);
    generate_rook_moves(board, moves);
    generate_queen_moves(board, moves);
    generate_king_moves(board, moves);

    return moves;
  }
  
  void MoveGenerator::generate_pawn_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_knight_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_bishop_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_rook_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_queen_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_king_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

} // namespace Engine