#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

namespace Engine {

  class MoveList;
  class Board;

  /**
   * @class MoveGenerator
   * @brief A stateless utility class used for move calculation.
   * @note All methods are static to avoid the overhead of object instantiation,
   * treating the generator as a pure functional component of the engine.
   */
  class MoveGenerator {
  public:
    static MoveList generate_legal_moves(const Board& board);
  private:
    /** @name Piece specific generation logic*/
    ///@{

    static void generate_pawn_moves(const Board& board, MoveList& moves);
    static void generate_knight_moves(const Board& board, MoveList& moves);
    static void generate_bishop_moves(const Board& board, MoveList& moves);
    static void generate_rook_moves(const Board& board, MoveList& moves);
    static void generate_queen_moves(const Board& board, MoveList& moves);
    static void generate_king_moves(const Board& board, MoveList& moves);
    
    ///@}
  };

} // namespace Engine

#endif