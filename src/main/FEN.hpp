#ifndef FEN_H
#define FEN_H

#include <string_view>
#include <string>
#include <vector>

namespace Engine {

  class Board;

  namespace IO {
  
    class Fen {
    public:
      /**
       * @brief Parses a FEN string and updates the board state.
       * @param board The board instance to modify.
       * @param fen The FEN string to parse.
       * @return true if parsing was successful.
       */
      static bool import(Board& board, std::string_view fen);
  
      /** 
       * @brief Converts the current board state into a FEN string.
      */
      static std::string export_fen(const Board& board);

    private:
      /**
       * @brief Splits a string_view into segments based on a delimiter.
       * @note This is O(N) and performs no new string allocations.
       */
      static std::vector<std::string_view> split(std::string_view str, char delimiter);

      /** @name Segment specific parsing logic*/
      ///@{

      inline static bool parse_pieces(Board& board, std::string_view seg);
      inline static bool parse_side_to_move(Board& board, std::string_view seg);
      inline static bool parse_castling_ability(Board& board, std::string_view seg);
      inline static bool parse_en_passant_target(Board& board, std::string_view seg);
      inline static bool parse_halfmove_clock(Board& board, std::string_view seg);
      inline static bool parse_fullmove_counter(Board& board, std::string_view seg);

      ///@}

      /** @name Segment specific exporting logic */
      ///@{

      inline static void export_piece_placement(std::stringstream& fen, const Board& board);
      inline static void export_active_color(std::stringstream& fen, const Board& board);
      inline static void export_castling_ability(std::stringstream& fen, const Board& board);
      inline static void export_en_passant_target(std::stringstream& fen, const Board& board);
      inline static void export_halfmove_clock(std::stringstream& fen, const Board& board);
      inline static void export_fullmove_counter(std::stringstream& fen, const Board& board);
      
      ///@}
    };
  
  } // namespace IO
  
} // namespace Engine

#endif