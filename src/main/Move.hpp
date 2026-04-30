#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <compare>

namespace Engine {

  /**
   * @enum MoveFlag
   * @brief 4-bit identifiers for special move types and metadata.
   * 
   * The flags are structured particulary to allow branchless logic and efficient move ordering:
   * - Bit 3: promotion bit
   * - Bit 2: capture bit
   * - Bit 1 and 0: special differentiators (promotions, castling side, pawn push types, etc.)
   * 
   * Because the capture bit is weighted highly, integer-based sorting naturally prioritizes
   * tactical moves during the search phase.
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
   * @brief Represents a chess move packed into a single 16-bit integer.
   * 
   * To maximize efficiency and minimize memory usage, moves are stored in following format:
   * | Bits  | Width | Description |
   * | :---  | :---  | :---        |
   * | 0-5   | 6     | To square (destination index 0-63) |
   * | 5-11  | 6     | From square (destination index 0-63) |
   * | 12-15 | 4     | Flags (\ref MoveFlag metadata) |
   *
   * @note By placing flags in the most significant bits, tactical moves represent higher integer values.
   * Therefore, a capture will aways result in a higher integer value than a quiet move.
   */
  class Move {
  public:
    // default constructor for an empty or invalid moves
    Move() noexcept = default;
    

    constexpr Move(uint16_t from, uint16_t to, MoveFlag flags) noexcept
      : data((static_cast<uint16_t>(flags) << 12) |
            ((to & 0x3F) << 6) | 
            (from & 0x3F)) {}
    
    // unpacking the represented data
    [[nodiscard]] constexpr uint16_t flag(void) const noexcept { return data >> 12; }
    [[nodiscard]] constexpr uint16_t to(void) const noexcept { return (data >> 6) & 0x3F; }
    [[nodiscard]] constexpr uint16_t from(void) const noexcept { return data & 0x3F; }

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