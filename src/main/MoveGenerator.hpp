#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include <cstdint>

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
    // default constructor
    MoveGenerator(void);
    
    // return list of all pseudo legal moves
    MoveList pseudo_legal_moves(const Board& board);

  private:
    /** @name Internal members*/
    ///@{

    // lookup table of bitboards for knight attacks
    uint64_t KNIGHT_ATTACK_TBL[64];
    
    ///@}

    /** @name Initialization logic*/
    ///@{

    // initialize lookup table for knights moves
    void init_knight_attacks(void);

    ///@}

    /** @name Piece specific generation logic*/
    ///@{

    static void pseudo_legal_pawn_moves(const Board& board, MoveList& moves);
    void pseudo_legal_knight_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_bishop_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_rook_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_queen_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_king_moves(const Board& board, MoveList& moves);
    
    ///@}
  };

} // namespace Engine

#endif