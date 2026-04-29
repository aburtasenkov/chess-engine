#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

namespace Engine {

  /**
   * @enum PieceType
   * @brief Helper enumerator used for lookup of the corresponding bitboards in \ref Board class.
   */
  enum PieceType
    : uint8_t 
  {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, ALL
  };

  /**
   * @enum Color
   * @brief Helper enumerator used for lookup of the corresponding bitboards' color in \ref Board class.
   */
  enum Color
    : uint8_t
  {
    WHITE, BLACK
  };

  /**
   * @struct Board
   * @brief Represents the physical chessboard using bitboards 
   * for every single chess piece.
   */
  class Board {
  public:
    Board(void);
    ~Board() = default;

  private:
    alignas(64) uint64_t pieces[2][7];

    uint64_t color_occupancy[2];
    uint64_t total_occupancy;
  };

}

#endif