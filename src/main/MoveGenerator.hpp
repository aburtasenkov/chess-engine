#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include <cstdint>

namespace Engine {

  class MoveList;
  class Board;

  /**
   * @class MoveGenerator
   * @brief A utility class used for move calculation.
   * @note The class needs to be initialized for knight move generation in order
   * to cache all possible knight moves.
   */
  class MoveGenerator {
  public:

    MoveGenerator(void) = delete;

    // interface for initialization
    static void init_tables(void);
    
    // return list of all pseudo legal moves
    static MoveList pseudo_legal_moves(const Board& board);

  private:
    /** @name Internal members*/
    ///@{

    // lookup table of bitboards for knight attacks
    static inline uint64_t KNIGHT_ATTACK_TBL[64] = {0};
    static inline bool is_initialized = false;
    
    ///@}

    /** @name Initialization logic*/
    ///@{

    // initialize lookup table for knights moves
    static void init_knight_attacks(void);

    ///@}

    /** @name Piece specific generation logic*/
    ///@{

    static void pseudo_legal_pawn_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_knight_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_bishop_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_rook_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_queen_moves(const Board& board, MoveList& moves);
    static void pseudo_legal_king_moves(const Board& board, MoveList& moves);
    
    ///@}
  };

} // namespace Engine

#endif