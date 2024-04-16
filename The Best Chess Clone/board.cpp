#include "board.h"
#include "piece.h"
#include "coordinates.h"
#include "constants.h"
#include <iostream>
#include <algorithm>
#include <array>
#include <utility>
#include <vector>
#include <memory>

Board::Board(Piece::Color player)
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
			char& letter{ m_matrix(i, j) };

			if (!Piece::isPiece(letter))
				continue;

			if (player == Piece::Color::Black) 
				letter = (letter == toupper(letter)) ? tolower(letter) : toupper(letter);
			
			auto& list{ (Piece::getColor(letter) == Piece::Color::White) ? m_whitePieces : m_blackPieces };

			list.push_back(Piece::toPiece(letter, { i, j }));
		}
	}
}

std::vector<const Piece*> Board::getPieces()
{
	std::vector<const Piece*> pieces{};

	pieces.reserve(m_whitePieces.size() + m_blackPieces.size());

	for (const auto& piece : m_whitePieces)
		pieces.push_back(piece.get());

	for (const auto& piece : m_blackPieces)
		pieces.push_back(piece.get());

	return pieces;
}


char Board::operator()(const Coordinates& coordinates) const
{
	return m_matrix(coordinates);
}

void Board::movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates)
{
	char& newSquare{ m_matrix(newCoordinates.x, newCoordinates.y) };

	if (Piece::isPiece(newSquare))
	{
		auto& list{ getListFromColor(Piece::getColor(newSquare)) };
		auto piece
		{ 
			std::find_if(list.begin(), list.end(), [&](const std::unique_ptr<Piece>& piece)
			{	 
				return piece->getCoordinates() == newCoordinates; 
			})
		};
		list.erase(piece);
	}

	newSquare = m_matrix(oldCoordinates.x, oldCoordinates.y);
	m_matrix(oldCoordinates.x, oldCoordinates.y) = 'x';

	const char letter{ m_matrix(oldCoordinates.x, oldCoordinates.y) };
	auto& list{ getListFromColor(Piece::getColor(letter)) };
	auto piece
	{
		std::find_if(list.begin(), list.end(), [&](const std::unique_ptr<Piece>& piece)
		{
			return piece->getCoordinates() == oldCoordinates;
		})
	};
	piece->get()->getCoordinates() = newCoordinates;
}

bool Board::isMovable(const Coordinates& coordinates)
{
	const char letter{ m_matrix(coordinates.x, coordinates.y) };
	return Piece::isPiece(letter) && Piece::getColor(letter) == Piece::Color::White;
}

bool Board::isOutOfBounds(const Coordinates& coordinates)
{
	return coordinates < Coordinates{ 0, 0 } || coordinates > Coordinates{ Constants::squaresPerLine - 1, Constants::squaresPerLine - 1 };
}

std::vector<std::unique_ptr<Piece>>& Board::getListFromColor(Piece::Color color)
{
	return (color == Piece::Color::White) ? m_whitePieces : m_blackPieces;
}