#ifndef MOVE_H
#define MOVE_H

#include "Constants.hpp"

#include <cstdint>
#include <compare>

namespace Engine {

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
    uint16_t data;
  };

} // namespace Engine

#endif