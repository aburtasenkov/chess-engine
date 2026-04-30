#ifndef BOARD_H
#define BOARD_H

#include "Constants.hpp"

#include <stdint.h>

namespace Engine {

  /**
   * @enum PieceType
   * @brief Helper enumerator used for lookup of the corresponding bitboards in \ref Engine::Board class.
   */
  enum PieceType
    : uint8_t 
  {
    PAWN = 0, KNIGHT, BISHOP, ROOK, QUEEN, KING, ALL
  };

  /**
   * @enum Color
   * @brief Helper enumerator used for lookup of the corresponding bitboards' color in \ref Engine::Board class.
   */
  enum Color
    : uint8_t
  {
    WHITE = 0, BLACK
  };

  /**
   * @struct Board
   * @brief Represents the physical chessboard using bitboards 
   * for every single chess piece.
   */
  class Board {
  public:
    Board(void) = default;
    ~Board() = default;

    [[nodiscard]] uint64_t get_piece_bitboard(Color color, PieceType piece) const { return pieces[color][piece]; }
    [[nodiscard]] uint64_t get_color_occupancy(Color color) const { return color_occupancy[color]; }
    [[nodiscard]] uint64_t get_total_occupancy(void) const { return total_occupancy; }

  private:
    // alignas(64) ensures that the member starts on a new cache line boundary
    // this prevents false sharing
    alignas(64) uint64_t pieces[2][7] = {
      {StartPos::WhitePawns, StartPos::WhiteKnights, StartPos::WhiteBishops, StartPos::WhiteRooks, StartPos::WhiteQueen, StartPos::WhiteKing, StartPos::WhiteAll},
      {StartPos::BlackPawns, StartPos::BlackKnights, StartPos::BlackBishops, StartPos::BlackRooks, StartPos::BlackQueen, StartPos::BlackKing, StartPos::BlackAll}
    };

    uint64_t color_occupancy[2] = {{StartPos::WhiteAll}, {StartPos::BlackAll}};
    uint64_t total_occupancy = color_occupancy[Color::WHITE] | color_occupancy[Color::BLACK];
  };

} // namespace Engine

#endif