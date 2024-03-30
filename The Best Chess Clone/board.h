#pragma once
#include "boardMatrix.h"
#include "piece.h"
#include <vector>

class Board
{
	public:

		Board();

		std::vector<Piece> getPieces();
		void movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates);
		bool isMovable(const Coordinates& coordinates);

	private:

		BoardMatrix m_matrix{ {} };
};

