#include "MoveGenerator.hpp"

#include "MoveList.hpp"

namespace Engine {

  MoveList MoveGenerator::generate_legal_moves(const Board& board) {
    (void)board;
    return MoveList();
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