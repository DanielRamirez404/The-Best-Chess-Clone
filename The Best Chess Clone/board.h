#pragma once
#include <vector>
#include "constants.h"
#include "piece.h"

class Board {

private:

	char matriz[Constants::squaresPerLine][Constants::squaresPerLine]{
		{'R','N','B','K','Q','B','N','R'},
		{ 'P','P','P','P','P','P','P','P' },
		{ 'x','x','x','x','x','x','x','x' },
		{ 'x','x','x','x','x','x','x','x' },
		{ 'x','x','x','x','x','x','x','x' },
		{ 'x','x','x','x','x','x','x','x' },
		{ 'p','p','p','p','p','p','p','p' },
		{ 'r','n','b','k','q','b','n','r' },
	};

public:

	void printMatrix();

	std::vector<Piece> getPieces();

};