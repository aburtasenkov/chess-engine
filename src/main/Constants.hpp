#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

namespace Engine {

  /**
   * @enum Square
   * @brief Represents the 64 squares of a chessboard using LERF mapping.
   * 
   * The engine uses Little-Endian Rank-File (LERF) mapping where:
   * - The files are ordered a-h (0-7).
   * - The ranks are ordered 1-8 (0-7).
   * 
   * Visual Mapping:
   * @code
   *   8 | 56 57 58 59 60 61 62 63
   *   7 | 48 49 50 51 52 53 54 55
   *   6 | 40 41 42 43 44 45 46 47
   *   5 | 32 33 34 35 36 37 38 39
   *   4 | 24 25 26 27 28 29 30 31
   *   3 | 16 17 18 19 20 21 22 23
   *   2 |  8  9 10 11 12 13 14 15
   *   1 |  0  1  2  3  4  5  6  7
   *     -------------------------
   *        a  b  c  d  e  f  g  h
   * @endcode
   */
  enum Square : uint8_t {
    SQ_A1 = 0, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
    SQ_NONE = 64  // invalid value
  };

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
    constexpr uint64_t WhitePawns   = (1ULL << SQ_A2) | (1ULL << SQ_B2) | (1ULL << SQ_C2) | (1ULL << SQ_D2) |
                                      (1ULL << SQ_E2) | (1ULL << SQ_F2) | (1ULL << SQ_G2) | (1ULL << SQ_H2);
    constexpr uint64_t WhiteKnights = (1ULL << SQ_B1) | (1ULL << SQ_G1);
    constexpr uint64_t WhiteBishops = (1ULL << SQ_C1) | (1ULL << SQ_F1);
    constexpr uint64_t WhiteRooks   = (1ULL << SQ_A1) | (1ULL << SQ_H1);
    constexpr uint64_t WhiteQueen   = (1ULL << SQ_D1);
    constexpr uint64_t WhiteKing    = (1ULL << SQ_E1);
    constexpr uint64_t WhiteAll     = WhitePawns | WhiteKnights | WhiteBishops | WhiteRooks | WhiteQueen | WhiteKing;

    constexpr uint64_t BlackPawns   = (1ULL << SQ_A7) | (1ULL << SQ_B7) | (1ULL << SQ_C7) | (1ULL << SQ_D7) |
                                      (1ULL << SQ_E7) | (1ULL << SQ_F7) | (1ULL << SQ_G7) | (1ULL << SQ_H7);
    constexpr uint64_t BlackKnights = (1ULL << SQ_B8) | (1ULL << SQ_G8);
    constexpr uint64_t BlackBishops = (1ULL << SQ_C8) | (1ULL << SQ_F8);
    constexpr uint64_t BlackRooks   = (1ULL << SQ_A8) | (1ULL << SQ_H8);
    constexpr uint64_t BlackQueen   = (1ULL << SQ_D8);
    constexpr uint64_t BlackKing    = (1ULL << SQ_E8);
    constexpr uint64_t BlackAll     = BlackPawns | BlackKnights | BlackBishops | BlackRooks | BlackQueen | BlackKing;
  }

} // namespace Engine

#endif