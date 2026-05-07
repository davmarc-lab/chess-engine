#pragma once

#include <string>
#include "../location.hpp"

namespace chess {
	namespace piece {
		/**
		 * @class Piece
		 * @brief Base class of a board piece.
		 * It handles pieces movement and attack.
		 */
		class Piece {
		public:
			Piece() = delete;
			/**
			 * @brief Instantiate standard piece.
			 *
			 * @param location the piece location on the board
			 * @param steps the number of steps the piece can do
			 */
			Piece(const Location &location, const short &steps) :
				m_location(location), m_steps(steps) {}

			virtual ~Piece() = default;

			Piece(Piece &&) = default;
			Piece(const Piece &) = default;
			Piece &operator=(Piece &&) = default;
			Piece &operator=(const Piece &) = default;

			/**
			 * @brief Retrieves piece Location.
			 *
			 * @return a Location of the piece.
			 */
			Location getLocation();

			/**
			 * @brief Sets the current piece Location if it's valid.
			 *
			 * @param loc new piece Location
			 */
			void setLocation(const Location &loc);

			/**
			 * @brief Retrieves the number of square the piece can do.
			 *
			 * @return a short representing the step's number.
			 */
			short getSteps();

			/**
			 * @brief Sets the step's number only if it's valid.
			 *
			 * @param steps the new step's amount
			 */
			void setSteps(const short &steps);

			/**
			 * @brief Standard debug string with all object values.
			 *
			 * @return a printable string.
			 */
			virtual std::string toString();

		protected:
			/// piece location
			Location m_location;
			/// piece step's number
			short m_steps;
		};

	} // namespace piece
} // namespace chess
