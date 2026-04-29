#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

namespace Engine {

  class MoveList;
  class Board;

  class MoveGenerator {
  public:
    static MoveList generate_legal_moves(const Board& board);
  private:
    static void generate_pawn_moves(const Board& board, const MoveList& moves);
    static void generate_knight_moves(const Board& board, const MoveList& moves);
    static void generate_bishop_moves(const Board& board, const MoveList& moves);
    static void generate_rook_moves(const Board& board, const MoveList& moves);
    static void generate_queen_moves(const Board& board, const MoveList& moves);
    static void generate_king_moves(const Board& board, const MoveList& moves);
  };

}

#endif