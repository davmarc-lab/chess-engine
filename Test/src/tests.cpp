// Testing
#include <iostream>

#include "engine/health.hpp"
#include "engine/location.hpp"
#include "engine/piece/piece.hpp"
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
		chess::Piece p1 = chess::Piece(l1, steps);
		std::cout << p1.toString() << "\n";
	}
	{
		chess::Location l1 = chess::Location('h', 8);
		chess::Piece p1 = chess::Piece(l1, steps);
		std::cout << p1.toString() << "\n";
	}
	{
		chess::Location l1 = chess::Location('a', -1);
		chess::Piece p1 = chess::Piece(l1, steps);
		std::cout << p1.toString() << "\n";
	}
	{
		chess::Location l1 = chess::Location('h', 0);
		chess::Piece p1 = chess::Piece(l1, steps);
		std::cout << p1.toString() << "\n";
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

	std::cout << "--- END ---\n";
	return 0;
}
