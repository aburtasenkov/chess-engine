#include "FEN.hpp"

#include "Board.hpp"

#include <charconv>
#include <sstream>

namespace Engine::IO {

  bool Fen::load(Board& board, std::string_view fen) {
    auto segments = split(fen, ' ');
    if (segments.size() < 4) return false;

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
    std::stringstream fen;

    export_piece_placement(fen, board);

    fen << ' ';
    export_active_color(fen, board);

    fen << ' ';
    export_castling_ability(fen, board);

    fen << ' ';
    export_en_passant_target(fen, board);

    fen << ' ';
    export_halfmove_clock(fen, board);

    fen << ' ';
    export_fullmove_counter(fen, board);

    return fen.str();
  }

  void Fen::export_piece_placement(std::stringstream& fen, const Board& board) {
    // maps colors and piece types from enumerator
    const static char piece_chars[2][6] = {
      {'P', 'N', 'B', 'R', 'Q', 'K'}, // white pieces
      {'p', 'n', 'b', 'r', 'q', 'k'}  // black pieces
    };

    for (int8_t rank = 7; rank >= 0; --rank) {
      uint8_t empty_count = 0;

      for (int8_t file = 0; file <= 7; ++file) {
        uint8_t sq = rank * 8 + file;
        uint64_t bit = 1ULL << sq;
        
        char piece_char = '\0'; // initialized to placeholder value

        for (uint8_t color = static_cast<uint8_t>(Color::WHITE); 
              color <= static_cast<uint8_t>(Color::BLACK); 
              ++color) 
        {
          for (uint8_t piece_type = static_cast<uint8_t>(PieceType::PAWN);
                piece_type <= static_cast<uint8_t>(PieceType::KING); 
                ++piece_type) 
          {
            if (board.get_piece_bitboard(static_cast<Color>(color), static_cast<PieceType>(piece_type)) & bit) {
              piece_char = piece_chars[color][piece_type];
              break;
            }
          }

          if (piece_char != '\0') {
            break;
          }
        }

        if (piece_char == '\0') {
          empty_count++;
        } else {
          if (empty_count > 0) {
            fen << static_cast<int>(empty_count); // cast to int to avoid interpretation as char
            empty_count = 0;
          }
          fen << piece_char;
        }
      }

      if (empty_count > 0) {
        fen << static_cast<int>(empty_count); // same as above
      }

      if (rank > 0) {
        fen << '/';
      }
    }
    return;
  }

  void Fen::export_active_color(std::stringstream& fen, const Board& board) {
    fen << (board.get_side_to_move() == Color::WHITE ? 'w' : 'b');
    return;
  }

  void Fen::export_castling_ability(std::stringstream& fen, const Board& board) {
    std::string fen_castling = "";
    uint8_t castling_rights = static_cast<uint8_t>(board.get_castling_rights());

    if (castling_rights == CastlingRights::NO_CASTLING) {
      fen_castling = '-';
    } else {
      if (castling_rights & CastlingRights::WHITE_OO) fen_castling += 'K';
      if (castling_rights & CastlingRights::WHITE_OOO) fen_castling += 'Q';
      if (castling_rights & CastlingRights::BLACK_OO) fen_castling += 'k';
      if (castling_rights & CastlingRights::BLACK_OOO) fen_castling += 'q';
    }

    fen << fen_castling;

    return;
  }

  void Fen::export_en_passant_target(std::stringstream& fen, const Board& board) {
    Square en_passant = board.get_en_passant_target();
    if (en_passant == Square::SQ_NONE) {
      fen << '-';
    } else {
      uint8_t square = static_cast<uint8_t>(en_passant);
      char file = 'a' + (square % 8);
      char rank = '1' + (square / 8);
      fen << file << rank;
    }

    return;
  }

  void Fen::export_halfmove_clock(std::stringstream& fen, const Board& board) {
    fen << board.get_halfmove_clock();
    return;
  }

  void Fen::export_fullmove_counter(std::stringstream& fen, const Board& board) {
    fen << board.get_fullmove_counter();
    return;
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
        // each rank must have 8 squares accounted for
        if (file != 8) return false;

        rank--;
        file = 0;
      } else if (isdigit(c)) {
        file += (c - '0');  // skip empty squares

        // check if digit pushed past the board
        if (file > 8) return false;
      } else {

        // boundary check: since rank is unsigned, it will wrap to 255
        if (file > 7 || rank > 7) return false;

        Color color = isupper(c) ? Color::WHITE : Color::BLACK;
        char lower_c = tolower(c);
        
        PieceType type;
        switch (lower_c) {
          case 'p': type = PieceType::PAWN;   break;
          case 'n': type = PieceType::KNIGHT; break;
          case 'b': type = PieceType::BISHOP; break;
          case 'r': type = PieceType::ROOK;   break;
          case 'q': type = PieceType::QUEEN;  break;
          case 'k': type = PieceType::KING;   break;
          default: return false;
        }

        board.set_piece(8 * rank + file, color, type);
        file++;
      }
    }
    return (rank == 0) && (file == 8);
  }

  bool Fen::parse_side_to_move(Board& board, std::string_view seg) {
    if (seg.length() != 1) return false;

    char c = seg[0];
    switch (c) {
      case 'w': board.set_side_to_move(Color::WHITE); return true;
      case 'b': board.set_side_to_move(Color::BLACK); return true;
      default: return false;
    }
  }

  bool Fen::parse_castling_ability(Board& board, std::string_view seg) {
    CastlingRights castling_rights = CastlingRights::NO_CASTLING;
    
    uint8_t length = 0;
    for (char c : seg) {
      if (length == 0 && c == '-') break;  // castling rights remain none

      if (length >= 4) return false;

      CastlingRights current = CastlingRights::NO_CASTLING;
      switch (c) {
        case 'K': current = CastlingRights::WHITE_OO;  break;
        case 'k': current = CastlingRights::BLACK_OO;  break;
        case 'Q': current = CastlingRights::WHITE_OOO; break;
        case 'q': current = CastlingRights::BLACK_OOO; break;
        default: return false;
      }

      castling_rights = static_cast<CastlingRights>(static_cast<uint8_t>(castling_rights) | current);
      length++;
    }

    board.set_castling_rights(castling_rights);

    return true;
  }

  bool Fen::parse_en_passant_target(Board& board, std::string_view seg) {
    if (seg == "-") return true; // en passant target is already set to none, since board is cleared

    if (seg.length() != 2) return false;

    uint8_t file = seg[0] - 'a';
    uint8_t rank = seg[1] - '1';

    if (file > 7 || (rank != 2 && rank != 5)) return false;

    board.set_en_passant_target(static_cast<Square>(rank * 8 + file));
    return true;
  }

  bool Fen::parse_halfmove_clock(Board& board, std::string_view seg) {
    uint16_t value = 0;
    auto [ptr, ec] = std::from_chars(seg.data(), seg.data() + seg.size(), value);

    if (ec != std::errc()) return false;

    board.set_halfmove_clock(value);
    return true;
  }

  bool Fen::parse_fullmove_counter(Board& board, std::string_view seg) {
    uint16_t value = 0;
    auto [ptr, ec] = std::from_chars(seg.data(), seg.data() + seg.size(), value);

    if (ec != std::errc()) return false;

    board.set_fullmove_counter(value);
    return true;  
  }

} // namespace Engine::IO