#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <compare>

namespace Engine {

  /**
   * @enum MoveFlag
   * @brief 4-bit identifiers for special move types.
   * Bit 3: promotion bit
   * Bit 2: capture bit
   * Bit 1 and 0: special bits (for differentiating promotions, etc.)
   * This avoids branch misprediction by allowing the developers to write branchless code.
   */
  enum MoveFlag 
    : uint16_t 
  {
    QUIET = 0,
    DOUBLE_PAWN_PUSH = 1,
    KING_CASTLE = 2,
    QUEEN_CASTLE = 3,
    CAPTURE = 4,
    EN_PASSANT = 5,
    // promotions (skips capture bit)
    P_KNIGHT = 8,
    P_BISHOP = 9,
    P_ROOK = 10,
    P_QUEEN = 11,
    // promotions + captures
    PC_KNIGHT = 12,
    PC_BISHOP = 13,
    PC_ROOK = 14,
    PC_QUEEN = 15
  };

  /**
   * @class Move
   * @brief Class representing chess moves. 
   * Bits 0-5: from square [values: 0-63]
   * Bits 6-11: to square [values: 0-63]
   * Bits 12-15: special \ref MoveFlag flags used for representing special info (captures, castling, promotions, etc.) [values: 0-15]
   * Since the flag is in the mathematical "thousands" place, a CAPTURE will always result in a higher integer value than a QUIET move.
   */
  class Move {
  public:
    // default constructor for an empty or invalid moves
    Move() noexcept = default;
    

    constexpr Move(uint16_t from, uint16_t to, MoveFlag flags) noexcept
      : data((static_cast<uint16_t>(flags) << 12) |
            ((from & 0x3F) << 6) | 
            (to & 0x3F)) {}
    
    // unpacking the represented data
    [[nodiscard]] constexpr uint16_t flag(void) const noexcept { return data >> 12; }
    [[nodiscard]] constexpr uint16_t from(void) const noexcept { return (data >> 6) & 0x3F; }
    [[nodiscard]] constexpr uint16_t to(void) const noexcept { return data & 0x3F; }

    [[nodiscard]] constexpr uint16_t get_raw(void) const noexcept { return data; }
    [[nodiscard]] constexpr bool is_null(void) const noexcept { return data == 0; }

    // comparison operators
    [[nodiscard]] bool operator==(const Move& other) const noexcept { return data == other.data; }
    [[nodiscard]] bool operator!=(const Move& other) const noexcept { return data != other.data; }

    [[nodiscard]] auto operator<=>(const Move& other) const noexcept { return data <=> other.data; }

  private:
    uint16_t data = 0x0;
  };

} // namespace Engine

#endif