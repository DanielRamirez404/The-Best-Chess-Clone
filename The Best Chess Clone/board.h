#pragma once
#include "boardMatrix.h"
#include "piece.h"
#include "coordinates.h"
#include <vector>

class Board
{
	public:

		Board();

		std::vector<Piece> getPieces();
		void movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates);
		bool isMovable(const Coordinates& coordinates);
		bool isKingMated(Piece::Color color);
		bool isKingChecked(Piece::Color color);


		Piece getPiece(const Coordinates& coordinates);

		std::vector<Coordinates> getAttacks(const Piece& piece);

	private:

		BoardMatrix m_matrix{ {} };
		std::vector<Piece> m_whitePieces{};
		std::vector<Piece> m_blackPieces{};

		bool isOutOfBounds(const Coordinates& coordinates);

		Piece::Type getType(char letter);
		Piece::Color getColor(char letter);

		bool isPinned(const Piece& piece);
		std::vector<Coordinates> getMoves(const Piece& piece);
		
};

/*
	todo: static check if it's out of bounds

*/