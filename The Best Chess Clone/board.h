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
		void movePieces(int i_1, int j_1, int i_2, int j_2);
		bool isMovable(int i, int j);
		void toMatrixCoord(int& x, int& y);

	private:

		BoardMatrix m_matrix{ {} };
};

