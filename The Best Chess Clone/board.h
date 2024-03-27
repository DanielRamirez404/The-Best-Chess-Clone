#pragma once
#include "boardMatrix.h"
#include "piece.h"
#include <vector>

class Board
{
	public:

		Board();

		void printMatrix();
		std::vector<Piece> getPieces();

	private:

		BoardMatrix m_matrix{ {} };
};