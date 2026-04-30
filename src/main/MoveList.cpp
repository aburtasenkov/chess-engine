#include "MoveList.hpp"

#include <cassert>

namespace Engine {

  void MoveList::push_back(Move move) {
    assert(sz < 256 && "MoveList overflow error");
    moves[sz++] = move;
  }

  Move MoveList::operator[](size_t index) const noexcept {
    assert(index < sz);
    return moves[index];
  }

} // namespace Engine