#include "board.h"
#include "piece.h"
#include "constants.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

Board::Board()
	: m_matrix
	{
		{
			'R','N','B','Q','K','B','N','R',
			'P','P','P','P','P','P','P','P',
			'x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x',
			'p','p','p','p','p','p','p','p',
			'r','n','b','q','k','b','n','r',
		}
	}
{
	m_whitePieces.reserve(Constants::piecesPerColor);
	m_blackPieces.reserve(Constants::piecesPerColor);
	
	for (int i = 0; i < Constants::squaresPerLine; i++) {
		for (int j = 0; j < Constants::squaresPerLine; j++) {
			if (m_matrix(i, j) == 'x') {
				continue;
			}

			Piece piece{ {i,j} };

			switch (tolower(m_matrix(i, j)))
			{
			case 'p':
				piece.type = Piece::Type::Pawn;
				break;
			case 'r':
				piece.type = Piece::Type::Rook;
				break;
			case 'n':
				piece.type = Piece::Type::Knight;
				break;
			case 'b':
				piece.type = Piece::Type::Bishop;
				break;
			case 'k':
				piece.type = Piece::Type::King;
				break;
			case 'q':
				piece.type = Piece::Type::Queen;
				break;
			}

			if (m_matrix(i, j) < 'a') {
				piece.color = Piece::Color::Black;
				m_blackPieces.push_back(piece);
			}
			else {
				piece.color = Piece::Color::White;
				m_whitePieces.push_back(piece);
			}
		}
	}
}

std::vector<Piece> Board::getPieces() 
{
	std::vector<Piece> list{};

		for (int i = 0; i < Constants::squaresPerLine; ++i)
		{
			for (int j = 0; j < Constants::squaresPerLine; ++j)
			{
				const char letter{ m_matrix(i, j) };

				if (letter == 'x')
					continue;

				Piece piece{ { i, j } };

				piece.color = (letter < 'a') ? Piece::Color::Black : Piece::Color::White;

				switch (tolower(letter))
				{
					case 'p':
						piece.type = Piece::Type::Pawn;
						break;
					case 'r':
						piece.type = Piece::Type::Rook;
						break;
					case 'n':
						piece.type = Piece::Type::Knight;
						break;
					case 'b':
						piece.type = Piece::Type::Bishop;
						break;
					case 'k':
						piece.type = Piece::Type::King;
						break;
					case 'q':
						piece.type = Piece::Type::Queen;
						break;
				}

				list.push_back(piece);
			}
		}

	return list;
}

void Board::movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates)
{
	m_matrix(newCoordinates.x, newCoordinates.y) = m_matrix(oldCoordinates.x, oldCoordinates.y);
	m_matrix(oldCoordinates.x, oldCoordinates.y) = 'x';
	Piece::Color color = (m_matrix(newCoordinates.x, newCoordinates.y) < 'a') ? Piece::Color::Black : Piece::Color::White;
	if (color == Piece::Color::Black) {
		for (auto& piece : m_blackPieces) {
			if (piece.coordinates == oldCoordinates) {
				piece.coordinates = newCoordinates;
				break;
			}
		}
	}
	else {
		for (auto& piece : m_whitePieces) { 
			if (piece.coordinates == oldCoordinates) {
				piece.coordinates = newCoordinates;
				break;
			}
		}
	}	
}

bool Board::isMovable(const Coordinates& coordinates)
{
	const char squareChar{ m_matrix(coordinates.x, coordinates.y) };
	return squareChar >= 'a' && squareChar != 'x';
}

bool Board::isKingChecked(Piece::Color color)
{
	const char king{ (color == Piece::Color::White) ? 'k' : 'K' };

	std::vector<char> rivalPieces{ {'P', 'N', 'Q', 'R', 'B'} };
	//To do: Tranform if it's white

	for (char piece : rivalPieces)
	{

	}

	return true;

}