#include "board.h"
#include "piece.h"
#include "coordinates.h"
#include "constants.h"
#include <iostream>
#include <algorithm>
#include <array>
#include <utility>
#include <vector>
#include <cctype>
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

			if (letter == 'x')
				continue;

			auto& list{ (getColor(letter) == Piece::Color::White) ? m_whitePieces : m_blackPieces };

			list.push_back(getPiece({ i, j }));
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

std::vector<std::unique_ptr<Piece>> Board::getPieces()
{
	std::vector<std::unique_ptr<Piece>> list{};

	for (int i = 0; i < Constants::squaresPerLine; ++i)
	{
		for (int j = 0; j < Constants::squaresPerLine; ++j)
		{
			if (m_matrix(i, j) == 'x')
				continue;
			
			std::unique_ptr<Piece> piece{ getPiece({ i, j }) };

			list.push_back(std::move(piece));
		}
	}

	return list;
}

void Board::movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates)
{
	char& newSquare{ m_matrix(newCoordinates.x, newCoordinates.y) };

	if (newSquare != 'x')
	{
		auto& list{ (getColor(newSquare) == Piece::Color::White) ? m_whitePieces : m_blackPieces };
		auto piece{ std::find_if(list.begin(), list.end(), [&](const std::unique_ptr<Piece>& piece) { return piece->getCoordinates() == newCoordinates; })};
		list.erase(piece);
	}

	newSquare = m_matrix(oldCoordinates.x, oldCoordinates.y);
	m_matrix(oldCoordinates.x, oldCoordinates.y) = 'x';

	const char letter{ m_matrix(oldCoordinates.x, oldCoordinates.y) };
	auto& list{ (getColor(letter) == Piece::Color::White) ? m_whitePieces : m_blackPieces };
	(*(std::find_if(list.begin(), list.end(), [&](const std::unique_ptr<Piece>& piece) { return piece->getCoordinates() == oldCoordinates; })))->getCoordinates() = newCoordinates;
}

bool Board::isMovable(const Coordinates& coordinates)
{
	const char letter{ m_matrix(coordinates.x, coordinates.y) };
	return letter != 'x' && getColor(letter) == Piece::Color::White;
}

bool Board::isOutOfBounds(const Coordinates& coordinates)
{
	return coordinates < Coordinates{ 0, 0 } || coordinates > Coordinates{ Constants::squaresPerLine - 1, Constants::squaresPerLine - 1 };
}

std::unique_ptr<Piece> Board::getPiece(const Coordinates& coordinates)
{
	char letter{ m_matrix(coordinates) };

	switch (getType(letter))
	{
		case Piece::Type::Pawn:
			return std::make_unique<Pawn>(coordinates, getColor(letter));
			break;
		case Piece::Type::Rook:
			return std::make_unique<Rook>(coordinates, getColor(letter));
			break;
		case Piece::Type::Knight:
			return std::make_unique<Knight>(coordinates, getColor(letter));
			break;
		case Piece::Type::Bishop:
			return std::make_unique<Bishop>(coordinates, getColor(letter));
			break;
		case Piece::Type::Queen:
			return std::make_unique<Queen>(coordinates, getColor(letter));
			break;
		case Piece::Type::King:
			return std::make_unique<King>(coordinates, getColor(letter));
			break;
	}

	return nullptr;
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
			Coordinates rightCapture{ piece->getCoordinates() + Coordinates{-1, 1}};
			Coordinates leftCapture{ piece->getCoordinates() + Coordinates{ -1, -1 } };

			if (!isOutOfBounds(rightCapture))
			{
				const char letter{ m_matrix(rightCapture) };
				if (letter == 'x' || getColor(letter) == Piece::Color::Black)
					attacks.push_back(std::move(rightCapture));
			}
			
			if (!isOutOfBounds(leftCapture))
			{
				const char letter{ m_matrix(leftCapture) };
				if (letter == 'x' || getColor(letter) == Piece::Color::Black)
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

					if (letter == 'x' || getColor(letter) == Piece::Color::Black)
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

				if (letter == 'x' || getColor(letter) == Piece::Color::Black)
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

					if (letter == 'x' || getColor(letter) == Piece::Color::Black)
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

				if (letter == 'x' || getColor(letter) == Piece::Color::Black)
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