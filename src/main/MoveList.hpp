#ifndef MOVELIST_H
#define MOVELIST_H

#include "Move.hpp"

#include <cstddef>

namespace Engine {

  class MoveList {
  public:
    using iterator = Move*;
    using const_iterator = const Move*;

    // iterators
    [[nodiscard]] iterator begin()  noexcept { return moves; };
    [[nodiscard]] iterator end()  noexcept { return moves + sz; }
    [[nodiscard]] const_iterator begin() const noexcept { return moves; };
    [[nodiscard]] const_iterator end() const noexcept { return moves + sz; }

    // capacity
    [[nodiscard]] size_t size(void) const noexcept { return sz; }
    [[nodiscard]] bool empty() const noexcept { return sz == 0; }

    // modifiers
    inline void push_back(Move move);

    inline void clear(void) { sz = 0; }

    [[nodiscard]] Move operator[](size_t index) const noexcept;

  private:
    // max amount of moves on any reachable position in chess is 218 
    // 256 is a safe power of 2 buffer
    // https://lichess.org/@/Tobs40/blog/why-a-reachable-position-can-have-at-most-218-playable-moves/a5xdxeqs
    Move moves[256];
    size_t sz = 0;
  };

} // namespace Engine

#endif