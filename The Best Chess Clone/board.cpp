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
	char& newSquare{ m_matrix(newCoordinates.x, newCoordinates.y) };

	if (newSquare != 'x')
	{
		std::vector<Piece>& list{ (getColor(newSquare) == Piece::Color::White) ? m_whitePieces : m_blackPieces };
		auto piece{ std::find_if(list.begin(), list.end(), [&](const Piece& piece) { return piece.coordinates == newCoordinates; }) };
		list.erase(piece);
	}

	newSquare = m_matrix(oldCoordinates.x, oldCoordinates.y);
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

bool Board::isOutOfBounds(const Coordinates& coordinates)
{
	return coordinates < Coordinates{ 0, 0 } || coordinates > Coordinates{ Constants::squaresPerLine - 1, Constants::squaresPerLine - 1 };
}

Piece Board::getPiece(const Coordinates& coordinates)
{
	char letter{ m_matrix(coordinates) };
	return { coordinates, getColor(letter), getType(letter)};
}

std::vector<Coordinates> Board::getAttacks(const Piece& piece)
{
	std::vector<Coordinates> attacks{};

	//todo: change the limitations to work with both sides
	//hint: forward and backwards (static?) variables depending on the player's color

	//todo: refactorize using POO and inheritance

	switch (piece.type)
	{
		case Piece::Type::Pawn:
		{
			const Coordinates rightCapture{ piece.coordinates + Coordinates{ -1, 1 } };
			const Coordinates leftCapture{ piece.coordinates + Coordinates{ -1, -1 } };

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
					Coordinates current{ piece.coordinates + Coordinates{ j, j } * directions[i] };

					//here, j, j should be a multiplication between the int and the coord object
					//todo: fix that

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
				Coordinates current{ piece.coordinates + directions[i] };

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
					Coordinates current{ piece.coordinates + Coordinates{ j, j } * directions[i] };

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
				Coordinates current{ piece.coordinates + directions[i] };

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
			attacks = getAttacks({ piece.coordinates, piece.color, Piece::Type::Rook });

			for (auto& attack : getAttacks({ piece.coordinates, piece.color, Piece::Type::Bishop }))
				attacks.push_back(std::move(attack));
		}
			break;
	}

	return attacks;
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