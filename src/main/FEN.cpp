#include "FEN.hpp"

#include "Board.hpp"

namespace Engine::IO {

  bool Fen::load(Board& board, std::string_view fen) {
    auto segments = split(fen, ' ');

    board.clear();

    if (!parse_pieces(board, segments[0]))            return false;
    if (!parse_side_to_move(board, segments[1]))      return false;
    if (!parse_castling_ability(board, segments[2]))  return false;
    if (!parse_en_passant_target(board, segments[3])) return false;

    // optional fields in some FEN syntaxes
    if (segments.size() > 4) parse_halfmove_clock(board, segments[4]);
    if (segments.size() > 5) parse_fullmove_counter(board, segments[5]);

    return true;
  }

  std::string Fen::export_fen(const Board& board) {
    (void)board;
    // TODO
    return "";
  }

  std::vector<std::string_view> Fen::split(std::string_view str, char delimiter) {
    std::vector<std::string_view> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string_view::npos) {
      tokens.push_back(str.substr(start, end - start));
      start = end + 1;
      end = str.find(delimiter, start);
    }

    // add final segment
    tokens.push_back(str.substr(start));

    return tokens;
  }

  bool Fen::parse_pieces(Board& board, std::string_view seg) {
    uint8_t rank = 7; // start at rank 8
    uint8_t file = 0; // start at file A

    for (char c : seg) {
      if (c == '/') { // rank separator
        rank--;
        file = 0;
      } else if (isdigit(c)) {
        file += (c - '0');  // skip empty squares
      } else {

        // boundary check
        if (file > 7 || rank < 0) return false;

        Color color = isupper(c) ? Color::WHITE : Color::BLACK;
        char lower_c = tolower(c);
        
        PieceType type = PieceType::NONE;
        switch (lower_c) {
          case 'p': type = PieceType::PAWN; break;
          case 'n': type = PieceType::KNIGHT; break;
          case 'b': type = PieceType::BISHOP; break;
          case 'r': type = PieceType::ROOK; break;
          case 'q': type = PieceType::QUEEN; break;
          case 'k': type = PieceType::KING; break;
          default: return false;
        }

        board.set_piece(8 * rank + file, color, type);
        file++;
      }
    }
    return true;
  }

  bool Fen::parse_side_to_move(Board& board, std::string_view seg) {
    Color side_to_move = Color::NONE;

    uint8_t length = 0;
    for (char c : seg) {
      if (length >= 1) return false; // FEN syntax check

      char lower_c = tolower(c);
      if (lower_c == 'w') side_to_move = Color::WHITE;
      else if (lower_c == 'b') side_to_move = Color::BLACK;

      length++;
    }

    if (side_to_move == Color::NONE) return false; // parsing failed

    board.set_side_to_move(side_to_move);
    return true;
  }

  bool Fen::parse_castling_ability(Board& board, std::string_view seg) {
    uint8_t length = 0;

    CastlingRights castling_rights = CastlingRights::NO_CASTLING;
    for (char c : seg) {
      if (length == 0 && c == '-') break;  // castling rights remain none

      if (length >= 4) return false;

      CastlingRights current = CastlingRights::NO_CASTLING;
      if (c == 'K') current = CastlingRights::WHITE_OO;
      else if (c == 'k') current = CastlingRights::BLACK_OO;
      else if (c == 'Q') current = CastlingRights::WHITE_OOO;
      else if (c == 'q') current = CastlingRights::BLACK_OOO;

      castling_rights = static_cast<CastlingRights>(static_cast<uint8_t>(castling_rights) | current);
    }

    board.set_castling_rights(castling_rights);

    return true;
  }

  bool Fen::parse_en_passant_target(Board& board, std::string_view seg) {
    (void)board; (void)seg; return false;
  }

  bool Fen::parse_halfmove_clock(Board& board, std::string_view seg) {
    (void)board; (void)seg; return false;
  }

  bool Fen::parse_fullmove_counter(Board& board, std::string_view seg) {
    (void)board; (void)seg; return false;
  }

}