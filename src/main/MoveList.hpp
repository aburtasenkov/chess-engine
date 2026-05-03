#ifndef MOVELIST_H
#define MOVELIST_H

#include "Move.hpp"

#include <cstddef>
#include <cassert>

namespace Engine {

  /**
   * @class MoveList
   * @brief A stack allocated collection of Move objects.
   * 
   * MoveList is designed specifically for the move generation part of the engine.
   * The class utilizes an array on the stack in order to avoid slow heap operations.
   * 
   * @note The capacity is capped at 256. While the maximum number of legal moves in 
   * a reachable chess position is 218, a power of 2 is used for alignment and safety.
   * @see https://lichess.org/@/Tobs40/blog/why-a-reachable-position-can-have-at-most-218-playable-moves/a5xdxeqs
   */
  class MoveList {
  public:
    using iterator = Move*;
    using const_iterator = const Move*;

    // iterators
    [[nodiscard]] iterator begin(void)  noexcept { return moves; };
    [[nodiscard]] iterator end(void)  noexcept { return moves + sz; }
    [[nodiscard]] const_iterator begin(void) const noexcept { return moves; };
    [[nodiscard]] const_iterator end(void) const noexcept { return moves + sz; }

    // capacity
    [[nodiscard]] size_t size(void) const noexcept { return sz; }
    [[nodiscard]] bool empty(void) const noexcept { return sz == 0; }

    // modifiers
    inline void push_back(Move move) {
      assert(sz < 256 && "MoveList overflow error");
      moves[sz++] = move;
    }

    inline void clear(void) { sz = 0; }

    [[nodiscard]] Move operator[](size_t index) const noexcept {
      assert(index < sz);
      return moves[index];
    }

  private:
    Move moves[256];
    size_t sz = 0;
  };

} // namespace Engine

#endif