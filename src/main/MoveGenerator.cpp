#include "MoveGenerator.hpp"

#include "Board.hpp"
#include "MoveList.hpp"

namespace Engine {

  // constants for calculating pawn captures
  static constexpr uint64_t FILE_A = 0x0101010101010101ULL;
  static constexpr uint64_t FILE_H = 0x8080808080808080ULL;

  // constant for calculating pawn promotions
  static constexpr uint64_t RANK_8 = 0xFF00000000000000ULL;

  // constant for calculating pawn double pushes
  static constexpr uint64_t RANK_3 = 0x0000000000FF0000ULL;

  // return index of the least significant bit that is "1" 
  static inline uint16_t lsb(uint64_t bitboard) {
    return static_cast<uint16_t>(__builtin_ctzll(bitboard));
  }

  static inline uint16_t pop_lsb(uint64_t& bitboard) {
    uint16_t i = lsb(bitboard);
    bitboard &= (bitboard - 1); // clear least significant bit, while leaving everything else untouched
    return i;
  }

  MoveList MoveGenerator::generate_legal_moves(const Board& board) {
    MoveList moves;

    // generating pseudo legal moves for now
    generate_pawn_moves(board, moves);
    generate_knight_moves(board, moves);
    generate_bishop_moves(board, moves);
    generate_rook_moves(board, moves);
    generate_queen_moves(board, moves);
    generate_king_moves(board, moves);

    return moves;
  }
  
  void MoveGenerator::generate_pawn_moves(const Board& board, MoveList& moves) {
    Color side_to_move = board.get_side_to_move();
    uint64_t pawns = board.get_piece_bitboard(side_to_move, PieceType::PAWN);

    uint64_t empty = board.get_empty_squares();
    uint64_t enemy = board.get_enemy_pieces();

    Square ep_square = board.get_en_passant_target();

    if (side_to_move == Color::WHITE) {
      // single push
      uint64_t single_push = (pawns << 8) & empty;
      uint64_t promo_push = single_push & RANK_8;
      uint64_t quiet_push = single_push & ~RANK_8;

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
      uint64_t double_push = ((single_push & RANK_3) << 8) & empty;
      while (double_push) {
        uint16_t to = pop_lsb(double_push);
        moves.push_back(Move(to - 16, to, DOUBLE_PAWN_PUSH));
      }

      // captures
      uint64_t capture_left = ((pawns & ~FILE_A) << 7) & enemy;
      uint64_t capture_right = ((pawns & ~FILE_H) << 9) & enemy;

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
        uint64_t ep_left = (pawns & ~FILE_A) << 7 & ep_bit;
        uint64_t ep_right = (pawns & ~FILE_H) << 9 & ep_bit;
        if (ep_left) {
          moves.push_back(Move(lsb(ep_left) - 7, static_cast<uint16_t>(ep_square), EN_PASSANT));
        }
        if (ep_right) {
          moves.push_back(Move(lsb(ep_right) - 9, static_cast<uint16_t>(ep_square), EN_PASSANT));
        }
      }

    } else {  // black turn

    }
  }

  void MoveGenerator::generate_knight_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_bishop_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_rook_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_queen_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

  void MoveGenerator::generate_king_moves(const Board& board, MoveList& moves) {
    (void)board;
    (void)moves;
  }

} // namespace Engine