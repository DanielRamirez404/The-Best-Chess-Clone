#include <iostream>
#include "Board.h"
#include "piece.h"
#include "constants.h"
#include <vector>

void Board::printMatrix() {
	for (int i = 0; i < Constants::squaresPerLine; i++) {
		for (int j = 0; j < Constants::squaresPerLine; j++) {
			std::cout << "[ " << matriz[i][j] << " ]";
		}
		std::cout << "\n";
	}
}

std::vector<Piece> Board::getPieces() {
	std::vector<Piece> list;
		for (int i = 0; i < Constants::squaresPerLine; i++) {
			for (int j = 0; j < Constants::squaresPerLine; j++) {

				if (matriz[i][j] == 'x') {
					continue;
				}

				if (matriz[i][j] == 'R') {
					Piece Pieza = { Piece::Black, Piece::Rook, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'N') {
					Piece Pieza = { Piece::Black, Piece::Knight, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'B') {
					Piece Pieza = { Piece::Black, Piece::Bishop, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}
				
				if (matriz[i][j] == 'K') {
					Piece Pieza = { Piece::Black, Piece::King, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'Q') {
					Piece Pieza = { Piece::Black, Piece::Queen, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'P') {
					Piece Pieza = { Piece::Black, Piece::Pawn, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'r') {
					Piece Pieza = { Piece::White, Piece::Rook, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'n') {
					Piece Pieza = { Piece::White, Piece::Knight, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'b') {
					Piece Pieza = { Piece::White, Piece::Bishop, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'k') {
					Piece Pieza = { Piece::White, Piece::King, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'q') {
					Piece Pieza = { Piece::White, Piece::Queen, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}

				if (matriz[i][j] == 'p') {
					Piece Pieza = { Piece::White, Piece::Pawn, i * Constants::squareSize, j * Constants::squareSize };
					list.push_back(Pieza);
				}


			}
		}
	return list;
}
