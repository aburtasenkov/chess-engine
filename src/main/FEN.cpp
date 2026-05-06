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
      if (c == '/') {
        rank--;
        file = 0;
      } else if (isdigit(c)) {
        file += (c - '0');  // skip empty squares
      } else {
        Color color = isupper(c) ? Color::WHITE : Color::BLACK;
        char lower_c = tolower(c);
        PieceType type;

        if      (lower_c == 'p') type = PieceType::PAWN;
        else if (lower_c == 'n') type = PieceType::KNIGHT;
        else if (lower_c == 'b') type = PieceType::BISHOP; 
        else if (lower_c == 'r') type = PieceType::ROOK;
        else if (lower_c == 'q') type = PieceType::QUEEN;
        else if (lower_c == 'k') type = PieceType::KING;

        board.set_piece(8 * rank + file, color, type);
        file++;
      }
    }
    return true;
  }

  bool Fen::parse_side_to_move(Board& board, std::string_view seg) {
    (void)board; (void)seg; return false;
  }

  bool Fen::parse_castling_ability(Board& board, std::string_view seg) {
    (void)board; (void)seg; return false;
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