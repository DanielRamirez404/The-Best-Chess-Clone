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

			if (!Piece::isPiece(letter))
				continue;

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


char Board::at(const Coordinates& coordinates)
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

std::vector<Coordinates> Board::getAttacks(const std::unique_ptr<Piece>& piece)
{
	std::vector<Coordinates> attacks{};

	//todo: change the limitations to work with both sides
	//hint: forward and backwards (static?) variables depending on the player's color

	//todo: refactorize using OOP and inheritance

	switch (piece->getType())
	{
		case Piece::Type::Pawn:
		{
			Coordinates rightCapture{ piece->getCoordinates() + Coordinates{-1, 1} };
			Coordinates leftCapture{ piece->getCoordinates() + Coordinates{ -1, -1 } };

			if (!isOutOfBounds(rightCapture))
			{
				const char letter{ m_matrix(rightCapture) };
				if (letter == 'x' || Piece::getColor(letter) == Piece::Color::Black)
					attacks.push_back(std::move(rightCapture));
			}
			
			if (!isOutOfBounds(leftCapture))
			{
				const char letter{ m_matrix(leftCapture) };
				if (letter == 'x' || Piece::getColor(letter) == Piece::Color::Black)
					attacks.push_back(std::move(leftCapture));
			}
		}
			break;

		case Piece::Type::Rook:
		{
			//refactorize this one and the next ones

			constexpr std::array<Coordinates, 4> directions{ { {0, 1}, {0, -1}, {1, 0}, {-1, 0} } };

			for (size_t i{ 0 }; i < directions.size(); ++i)
			{
				for (int j{ 1 }; j < Constants::squaresPerLine; ++j)
				{
					Coordinates current{ piece->getCoordinates() + Coordinates{ j, j } * directions[i] };

					if (isOutOfBounds(current))
						break;

					char letter{ m_matrix(current) };

					if (letter == 'x' || Piece::getColor(letter) == Piece::Color::Black)
						attacks.push_back(std::move(current));

					if (letter != 'x')
						break;
				}
			}
		}
			break;

		case Piece::Type::Knight:
		{
			constexpr std::array<Coordinates, 8> directions{ { {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2} } };

			for (size_t i{ 0 }; i < directions.size(); ++i)
			{
				Coordinates current{ piece->getCoordinates() + directions[i] };

				if (isOutOfBounds(current))
					continue;

				char letter{ m_matrix(current) };

				if (letter == 'x' || Piece::getColor(letter) == Piece::Color::Black)
					attacks.push_back(std::move(current));
			}
		}
			break;

		case Piece::Type::Bishop:
		{
			constexpr std::array<Coordinates, 4> directions{ { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} } };

			for (size_t i{ 0 }; i < directions.size(); ++i)
			{
				for (int j{ 1 }; j < Constants::squaresPerLine; ++j)
				{
					Coordinates current{ piece->getCoordinates() + Coordinates{ j, j } * directions[i] };

					if (isOutOfBounds(current))
						break;

					char letter{ m_matrix(current) };

					if (letter == 'x' || Piece::getColor(letter) == Piece::Color::Black)
						attacks.push_back(std::move(current));

					if (letter != 'x')
						break;
				}
			}
		}
			break;
		case Piece::Type::King:
		{
			constexpr std::array<Coordinates, 8> directions{ { {0, 1}, {0, -1}, {1, 0}, {-1, 0}, { 1, 1 }, {-1, -1}, {-1, 1}, {1, -1} }};
			
			for (size_t i{ 0 }; i < directions.size(); ++i)
			{
				Coordinates current{ piece->getCoordinates() + directions[i] };

				if (isOutOfBounds(current))
					continue;

				char& letter{ m_matrix(current) };

				if (letter == 'x' || Piece::getColor(letter) == Piece::Color::Black)
					attacks.push_back(std::move(current));
			}
		}
			break;
		case Piece::Type::Queen:
		{
			attacks = getAttacks( std::make_unique<Rook>(piece->getCoordinates(), piece->getColor()) );

			for (auto& attack : getAttacks( std::make_unique<Bishop>(piece->getCoordinates(), piece->getColor())) )
				attacks.push_back(std::move(attack));
		}
			break;
	}

	return attacks;
}

std::vector<std::unique_ptr<Piece>>& Board::getListFromColor(Piece::Color color)
{
	return (color == Piece::Color::White) ? m_whitePieces : m_blackPieces;
}