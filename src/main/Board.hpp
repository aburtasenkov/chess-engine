#ifndef BOARD_H
#define BOARD_H

#include "Constants.hpp"

#include <stdint.h>
#include <cassert>

namespace Engine {

  /**
   * @enum PieceType
   * @brief Enumerator for piece identification and internal \ref Engine::Board array indexing.
   */
  enum PieceType
    : uint8_t 
  {
    PAWN = 0, KNIGHT, BISHOP, ROOK, QUEEN, KING, ALL
  };

  /**
   * @enum Color
   * @brief Enumerator for side identification.
   * 
   * Used as the primary index for bitboard arrays to differentiate between
   * white and black piece sets.
   */
  enum Color
    : uint8_t
  {
    WHITE = 0, BLACK, BOTH
  };

  /**
   * @struct Board
   * @brief Representation of a physical chessboard state.
   * 
   * The Board class utilizes bitboards (64-bit integers) to represent piece positions.
   * Each bit corresponds to a square on the chessboard, allowing for extremely fast
   * move generation and position evaluation using bitwise operations.
   * 
   * @note This class is optimized for cache efficiency and memory alignment 
   * to prevent false sharing in multithreaded environments.
   */
  class Board {
  public:
    Board(void) = default;
    ~Board() = default;

    [[nodiscard]] uint64_t get_piece_bitboard(Color color, PieceType piece) const { return pieces[color][piece]; }
    [[nodiscard]] uint64_t get_color_occupancy(Color color) const { return color_occupancy[color]; }
    [[nodiscard]] uint64_t get_total_occupancy(void) const { return color_occupancy[Color::BOTH]; }

    void set_piece(uint8_t square, Color color, PieceType piece) {
      assert(square < 64 && "square index is too high!");
      uint64_t bit = 1ULL << square;

      pieces[color][piece] |= bit;
      color_occupancy[color] |= bit;
      color_occupancy[Color::BOTH] |= bit;
    }

  private:
    // alignas(64) ensures that the member starts on a new cache line boundary
    // this prevents false sharing
    alignas(64) uint64_t pieces[2][7] = {
      {StartPos::WhitePawns, StartPos::WhiteKnights, StartPos::WhiteBishops, StartPos::WhiteRooks, StartPos::WhiteQueen, StartPos::WhiteKing, StartPos::WhiteAll},
      {StartPos::BlackPawns, StartPos::BlackKnights, StartPos::BlackBishops, StartPos::BlackRooks, StartPos::BlackQueen, StartPos::BlackKing, StartPos::BlackAll}
    };

    uint64_t color_occupancy[3] = {
                                  {StartPos::WhiteAll}, 
                                  {StartPos::BlackAll}, 
                                  {StartPos::WhiteAll | StartPos::BlackAll}
    };

    CastlingRights castling_rights = CastlingRights::ALL_CASTLING;
    Color side_to_move = Color::WHITE;
    Square en_pessant = Square::SQ_NONE;  // allowed square to capture an en pessant move
  };

} // namespace Engine

#endif