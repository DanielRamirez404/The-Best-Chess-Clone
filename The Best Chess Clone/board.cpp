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
	
	for (int i = 0; i < Constants::squaresPerLine; i++)
	{
		for (int j = 0; j < Constants::squaresPerLine; j++)
		{
			const char letter{ m_matrix(i, j) };

			if (letter == 'x')
				continue;

			std::vector<Piece>& list{ (getColor(letter) == Piece::Color::White) ? m_whitePieces : m_blackPieces };
			list.push_back( { { i, j }, getColor(letter), getType(letter) } );
		}
	}
}

Piece::Type Board::getType(char letter)
{
	switch (tolower(letter))
	{
		case 'r':
			return Piece::Type::Rook;
			break;
		case 'n':
			return Piece::Type::Knight;
			break;
		case 'b':
			return Piece::Type::Bishop;
			break;
		case 'k':
			return Piece::Type::King;
			break;
		case 'q':
			return Piece::Type::Queen;
			break;
	}

	return Piece::Type::Pawn;
}

Piece::Color Board::getColor(char letter)
{
	return (letter < 'a') ? Piece::Color::Black : Piece::Color::White;
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

				list.push_back( { { i, j }, getColor(letter), getType(letter) } );
			}
		}

	return list;
}

void Board::movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates)
{
	m_matrix(newCoordinates.x, newCoordinates.y) = m_matrix(oldCoordinates.x, oldCoordinates.y);
	m_matrix(oldCoordinates.x, oldCoordinates.y) = 'x';

	const char letter{ m_matrix(oldCoordinates.x, oldCoordinates.y) };
	std::vector<Piece>& list{ (getColor(letter) == Piece::Color::White) ? m_whitePieces : m_blackPieces };
	std::find_if(list.begin(), list.end(), [&](const Piece& piece) { return piece.coordinates == oldCoordinates; })->coordinates = newCoordinates;
}

bool Board::isMovable(const Coordinates& coordinates)
{
	const char letter{ m_matrix(coordinates.x, coordinates.y) };
	return letter != 'x' && getColor(letter) == Piece::Color::White;
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