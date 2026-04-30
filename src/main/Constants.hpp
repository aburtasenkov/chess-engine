#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

namespace Engine {

  /**
   * @namespace StartPos
   * @brief Initial bitboard values for a standard game of chess.
   * 
   * Bitboards are mapped using the Little-Endian Rank-File mapping, where:
   * - Bit 0 represents a1
   * - Bit 7 represents a8
   * - Bit 63 represents h8
   */
  namespace StartPos {
    constexpr uint64_t WhitePawns   = 0x000000000000FF00ULL;
    constexpr uint64_t WhiteKnights = 0x0000000000000042ULL;
    constexpr uint64_t WhiteBishops = 0x0000000000000024ULL;
    constexpr uint64_t WhiteRooks   = 0x0000000000000081ULL;
    constexpr uint64_t WhiteQueen   = 0x0000000000000008ULL;
    constexpr uint64_t WhiteKing    = 0x0000000000000010ULL;
    constexpr uint64_t WhiteAll     = 0x000000000000FFFFULL;

    constexpr uint64_t BlackPawns   = 0x00FF000000000000ULL;
    constexpr uint64_t BlackKnights = 0x4200000000000000ULL;
    constexpr uint64_t BlackBishops = 0x2400000000000000ULL;
    constexpr uint64_t BlackRooks   = 0x8100000000000000ULL;
    constexpr uint64_t BlackQueen   = 0x0800000000000000ULL;
    constexpr uint64_t BlackKing    = 0x1000000000000000ULL;
    constexpr uint64_t BlackAll     = 0xFFFF000000000000ULL;
  }

} // namespace Engine

#endif