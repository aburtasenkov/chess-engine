#include "MoveGenerator.hpp"

#include "Board.hpp"
#include "MoveList.hpp"
#include "Constants.hpp"

namespace Engine {

  // return index of the least significant bit that is "1" 
  static inline uint16_t lsb(uint64_t bitboard) {
    return static_cast<uint16_t>(__builtin_ctzll(bitboard));
  }

  static inline uint16_t pop_lsb(uint64_t& bitboard) {
    uint16_t i = lsb(bitboard);
    bitboard &= (bitboard - 1); // clear least significant bit, while leaving everything else untouched
    return i;
  }

  MoveGenerator::MoveGenerator(void) {
    init_knight_attacks();
  }

  MoveList MoveGenerator::pseudo_legal_moves(const Board& board) {
    MoveList moves;

    // generating pseudo legal moves for now
    pseudo_legal_pawn_moves(board, moves);
    pseudo_legal_knight_moves(board, moves);
    pseudo_legal_bishop_moves(board, moves);
    pseudo_legal_rook_moves(board, moves);
    pseudo_legal_queen_moves(board, moves);
    pseudo_legal_king_moves(board, moves);

    return moves;
  }
  
  void MoveGenerator::pseudo_legal_pawn_moves(const Board& board, MoveList& moves) {
    Color side_to_move = board.get_side_to_move();
    uint64_t pawns = board.get_piece_bitboard(side_to_move, PieceType::PAWN);

    uint64_t empty = board.get_empty_squares();
    uint64_t enemy = board.get_enemy_pieces();

    Square ep_square = board.get_en_passant_target();

    if (side_to_move == Color::WHITE) {
      // single push
      uint64_t single_push = (pawns << 8) & empty;
      uint64_t promo_push = single_push & Grid::RANK_8;
      uint64_t quiet_push = single_push & ~Grid::RANK_8;

      while (quiet_push) {
        uint16_t to = pop_lsb(quiet_push);
        moves.push_back(Move(to - 8, to, QUIET));
      }

      while (promo_push) {
        uint16_t to = pop_lsb(promo_push);
        moves.push_back(Move(to - 8, to, P_BISHOP));
        moves.push_back(Move(to - 8, to, P_KNIGHT));
        moves.push_back(Move(to - 8, to, P_QUEEN));
        moves.push_back(Move(to - 8, to, P_ROOK));
      }

      // double pushes
      // only from rank 2 (when single push is on rank 3) and intermediate square is empty
      uint64_t double_push = ((single_push & Grid::RANK_3) << 8) & empty;
      while (double_push) {
        uint16_t to = pop_lsb(double_push);
        moves.push_back(Move(to - 16, to, DOUBLE_PAWN_PUSH));
      }

      // captures
      uint64_t capture_left = ((pawns & ~Grid::FILE_A) << 7) & enemy;
      uint64_t capture_right = ((pawns & ~Grid::FILE_H) << 9) & enemy;

      auto serialize_captures = [&](uint64_t cap_mask, int16_t offset) {
        while (cap_mask) {
          uint16_t to = pop_lsb(cap_mask);
          if (to >= 56) { // to promotion rank
            moves.push_back(Move(to - offset, to, PC_BISHOP));
            moves.push_back(Move(to - offset, to, PC_KNIGHT));
            moves.push_back(Move(to - offset, to, PC_QUEEN));
            moves.push_back(Move(to - offset, to, PC_ROOK));
          } else {
            moves.push_back(Move(to - offset, to, CAPTURE));
          }
        }
      };
      serialize_captures(capture_left, 7);
      serialize_captures(capture_right, 9);

      // en passant
      if (ep_square != Square::SQ_NONE) {
        uint64_t ep_bit = 1ULL << static_cast<uint16_t>(ep_square); // convert square to bitboard representation
        uint64_t ep_left = (pawns & ~Grid::FILE_A) << 7 & ep_bit;
        uint64_t ep_right = (pawns & ~Grid::FILE_H) << 9 & ep_bit;
        if (ep_left) {
          moves.push_back(Move(lsb(ep_left) - 7, static_cast<uint16_t>(ep_square), EN_PASSANT));
        }
        if (ep_right) {
          moves.push_back(Move(lsb(ep_right) - 9, static_cast<uint16_t>(ep_square), EN_PASSANT));
        }
      }

    } else {  // black turn
      // single push
      uint64_t single_push = (pawns >> 8) & empty;
      uint64_t promo_push = single_push & Grid::RANK_1;
      uint64_t quiet_push = single_push & ~Grid::RANK_1;

      while (quiet_push) {
        uint16_t to = pop_lsb(quiet_push);
        moves.push_back(Move(to + 8, to, QUIET));
      }

      while (promo_push) {
        uint16_t to = pop_lsb(promo_push);
        moves.push_back(Move(to + 8, to, P_BISHOP));
        moves.push_back(Move(to + 8, to, P_KNIGHT));
        moves.push_back(Move(to + 8, to, P_QUEEN));
        moves.push_back(Move(to + 8, to, P_ROOK));
      }

      // double pushes
      // only from rank 7 (when single push is on rank 6) and intermediate square is empty
      uint64_t double_push = ((single_push & Grid::RANK_6) >> 8) & empty;
      while (double_push) {
        uint16_t to = pop_lsb(double_push);
        moves.push_back(Move(to + 16, to, DOUBLE_PAWN_PUSH));
      }

      // captures
      uint64_t capture_left = ((pawns & ~Grid::FILE_A) >> 9) & enemy;
      uint64_t capture_right = ((pawns & ~Grid::FILE_H) >> 7) & enemy;

      auto serialize_captures = [&](uint64_t cap_mask, int16_t offset) {
        while (cap_mask) {
          uint16_t to = pop_lsb(cap_mask);
          if (to <= 7) { // to promotion rank
            moves.push_back(Move(to + offset, to, PC_BISHOP));
            moves.push_back(Move(to + offset, to, PC_KNIGHT));
            moves.push_back(Move(to + offset, to, PC_QUEEN));
            moves.push_back(Move(to + offset, to, PC_ROOK));
          } else {
            moves.push_back(Move(to + offset, to, CAPTURE));
          }
        }
      };
      serialize_captures(capture_left, 9);
      serialize_captures(capture_right, 7);

      // en passant
      if (ep_square != Square::SQ_NONE) {
        uint64_t ep_bit = 1ULL << static_cast<uint16_t>(ep_square); // convert square to bitboard representation
        uint64_t ep_left = (pawns & ~Grid::FILE_A) >> 9 & ep_bit;
        uint64_t ep_right = (pawns & ~Grid::FILE_H) >> 7 & ep_bit;
        if (ep_left) {
          moves.push_back(Move(lsb(ep_left) + 9, static_cast<uint16_t>(ep_square), EN_PASSANT));
        }
        if (ep_right) {
          moves.push_back(Move(lsb(ep_right) + 7, static_cast<uint16_t>(ep_square), EN_PASSANT));
        }
      }
    }
  }

  void MoveGenerator::init_knight_attacks(void) {
    for (uint8_t square = 0; square < 64; ++square) {
      uint64_t bitboard = 1ULL << square;
      uint64_t attacks = 0ULL;

      // shifts moving up the bitboard
      attacks |= (bitboard << 17) & ~Grid::FILE_A;
      attacks |= (bitboard << 15) & ~Grid::FILE_H;
      attacks |= (bitboard << 10) & ~Grid::FILE_AB;
      attacks |= (bitboard << 6) & ~Grid::FILE_GH;

      // shifts moving down the bitboard
      attacks |= (bitboard << 17) & ~Grid::FILE_H;
      attacks |= (bitboard << 15) & ~Grid::FILE_A;
      attacks |= (bitboard << 10) & ~Grid::FILE_GH;
      attacks |= (bitboard << 6) & ~Grid::FILE_AB;

      KNIGHT_ATTACK_TBL[square] = attacks;
    }
  }

  void MoveGenerator::pseudo_legal_knight_moves(const Board& board, MoveList& moves) {
    Color side_to_move = board.get_side_to_move();
    uint64_t knights = board.get_piece_bitboard(side_to_move, PieceType::KNIGHT);

    uint64_t friendly_pieces = board.get_color_bitboard(side_to_move);
    uint64_t hostile_pieces = board.get_enemy_pieces();

    while (knights) {
      uint16_t from = pop_lsb(knights);

      uint64_t attacks = KNIGHT_ATTACK_TBL[from] & ~friendly_pieces;

      while (attacks) {
        uint16_t to = pop_lsb(attacks);

        if ((1ULL << to) & hostile_pieces) {
          moves.push_back(Move(from, to, CAPTURE));
        } else {
          moves.push_back(Move(from, to, QUIET));
        }
      }
    }
  }

  void MoveGenerator::pseudo_legal_bishop_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::pseudo_legal_rook_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::pseudo_legal_queen_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::pseudo_legal_king_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

} // namespace Engine