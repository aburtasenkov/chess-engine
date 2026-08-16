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
       * @param fen The FEN string to load.
       * @return true if parsing was successful.
       */
      static bool load(Board& board, std::string_view fen);
  
      /** 
       * @brief Converts the current board state into a FEN string.
      */
      static std::string save(const Board& board);

    private:
      /**
       * @brief Splits a string_view into segments based on a delimiter.
       * @note This is O(N) and performs no new string allocations.
       */
      static std::vector<std::string_view> split(std::string_view str, char delimiter);

      /** @name Segment specific parsing logic*/
      ///@{

      inline static bool load_pieces(Board& board, std::string_view seg);
      inline static bool load_side_to_move(Board& board, std::string_view seg);
      inline static bool load_castling_ability(Board& board, std::string_view seg);
      inline static bool load_en_passant_target(Board& board, std::string_view seg);
      inline static bool load_halfmove_clock(Board& board, std::string_view seg);
      inline static bool load_fullmove_counter(Board& board, std::string_view seg);

      ///@}

      /** @name Segment specific exporting logic */
      ///@{

      inline static void save_piece_placement(std::stringstream& fen, const Board& board);
      inline static void save_active_color(std::stringstream& fen, const Board& board);
      inline static void save_castling_ability(std::stringstream& fen, const Board& board);
      inline static void save_en_passant_target(std::stringstream& fen, const Board& board);
      inline static void save_halfmove_clock(std::stringstream& fen, const Board& board);
      inline static void save_fullmove_counter(std::stringstream& fen, const Board& board);
      
      ///@}
    };
  
  } // namespace IO
  
} // namespace Engine

#endif