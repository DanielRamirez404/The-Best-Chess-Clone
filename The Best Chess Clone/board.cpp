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
{}

void Board::printMatrix()
{
	for (int i = 0; i < Constants::squaresPerLine; ++i) 
	{
		for (int j = 0; j < Constants::squaresPerLine; ++j) 
			std::cout << "[ " << m_matrix(i, j) << " ]";

		std::cout << "\n";
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

				Piece piece{ Piece::Color::White, Piece::Type::Pawn, i * Constants::squareSize, j * Constants::squareSize };

				if (letter < 'a')
					piece.color = Piece::Color::Black;

				switch (tolower(letter))
				{
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

void Board::movePieces(int i_1, int j_1, int i_2, int j_2)
{
	m_matrix(i_2, j_2) = m_matrix(i_1, j_1);
	m_matrix(i_1, j_1) = 'x';
}

bool Board::isMovable(int i, int j)
{
	return m_matrix(i, j) >= 'a' && m_matrix(i, j) != 'x';
}

void Board::toMatrixCoord(int& x, int& y)
{
	for (int i{0}; i <= Constants::squaresPerLine; ++i)
	{
		if (x < i * Constants::squareSize)
		{
			x = i - 1;
			break;
		}
	}

	for (int j{ 0 }; j <= Constants::squaresPerLine; ++j)
	{
		if (y < j * Constants::squareSize)
		{
			y = j - 1;
			break;
		}
	}

	std::swap(x, y);
}