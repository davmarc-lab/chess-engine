// Testing
#include <iostream>

#include "engine/board/board.hpp"
#include "engine/health.hpp"
#include "engine/location.hpp"

#include "engine/piece/bishop.hpp"
#include "engine/piece/king.hpp"
#include "engine/piece/knight.hpp"
#include "engine/piece/pawn.hpp"
#include "engine/piece/piece.hpp"
#include "engine/piece/queen.hpp"
#include "engine/piece/rook.hpp"

#include "engine/utils.hpp"

void testHealth() {
	chess::checkhealth();
}

void testLocations() {
	{
		chess::Location l1 = chess::Location('a', 2);
		std::cout << "(a2) -> " << pcond(l1.isValid()) << "\n";
	}
	{
		chess::Location l1 = chess::Location('h', 8);
		std::cout << "(h8) -> " << pcond(l1.isValid()) << "\n";
	}
	{
		chess::Location l1 = chess::Location('a', -1);
		std::cout << "(a9) -> " << pcond(l1.isValid()) << "\n";
	}
	{
		chess::Location l1 = chess::Location('h', 0);
		std::cout << "(h0) -> " << pcond(l1.isValid()) << "\n";
	}
}

void testPieces() {
	short steps = 1;
	{
		chess::Location l1 = chess::Location('a', 2);
		chess::piece::Pawn p1 = chess::piece::Pawn(l1);
		std::cout << p1.toString() << "\n";
	}
	{
		chess::Location l1 = chess::Location('h', 8);
		chess::piece::King p1 = chess::piece::King(l1);
		std::cout << p1.toString() << "\n";
	}
	{
		chess::Location l1 = chess::Location('a', -1);
		chess::piece::Queen p1 = chess::piece::Queen(l1);
		std::cout << p1.toString() << "\n";
	}
	{
		chess::Location l1 = chess::Location('h', 0);
		chess::piece::Knight p1 = chess::piece::Knight(l1);
		std::cout << p1.toString() << "\n";
	}
}

void testBoard() {
	chess::board::BoardSettings settings = {};
	chess::Location l1 = chess::Location('a', 0);

	{
		chess::board::Board b1 = chess::board::Board(settings);

		std::cout << b1.toString() << "\n\n";
		std::cout << "Max Pieces: " << b1.maxPieces() << "\n\n";
		std::cout << "Verifying template classes...\n";

		b1.addPiece<chess::piece::Piece>(l1, 1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::King>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Queen>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Bishop>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Knight>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Rook>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Pawn>(l1);
	}

	settings.sides = 1;
	settings.pieces = 3;
	std::cout << "\nUpdated settings, recreating board...\n";
	{
		chess::board::Board b1 = chess::board::Board(settings);

		std::cout << b1.toString() << "\n\n";
		std::cout << "Max Pieces: " << b1.maxPieces() << "\n\n";
		std::cout << "Verifying template classes...\n";

		b1.addPiece<chess::piece::Piece>(l1, 1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::King>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Queen>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
		b1.addPiece<chess::piece::Bishop>(l1);
		std::cout << "\tcurrent -> " << b1.numCurrentPieces() << "\n";
	}
}

int main(int argc, char *argv[]) {
	std::cout << "--- START ---\n\n";

	std::cout << "[ ] Health\n";
	testHealth();
	std::cout << "\n";

	std::cout << "[ ] Location\n";
	testLocations();
	std::cout << "\n";

	std::cout << "[ ] Piece\n";
	testPieces();
	std::cout << "\n";

	std::cout << "[ ] Board\n";
	testBoard();
	std::cout << "\n";

	std::cout << "--- END ---\n";
	return 0;
}
