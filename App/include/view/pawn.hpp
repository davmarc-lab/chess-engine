#pragma once

namespace view {

	class Pawn {
	public:
		Pawn() = default;

		void onAttach();

		unsigned int getId() const;

		Pawn(Pawn &&) = default;
		Pawn(const Pawn &) = default;
		Pawn &operator=(Pawn &&) = default;
		Pawn &operator=(const Pawn &) = default;

		~Pawn() = default;

	private:
		unsigned int m_id;
	};

} // namespace view
