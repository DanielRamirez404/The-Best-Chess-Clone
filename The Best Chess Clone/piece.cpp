#include "piece.h"
#include "coordinates.h"
#include "constants.h"
#include <memory>
#include <cctype>

Piece::Piece(const Coordinates& coordinates, Color color)
	: m_coordinates{ coordinates }, m_color{ color } {}

Piece::Color Piece::getColor() const
{
	return m_color;
}

const Coordinates& Piece::getCoordinates() const
{
	return m_coordinates;
}

Coordinates& Piece::getCoordinates()
{
	return m_coordinates;
}

Piece::Color Piece::getColor(char letter)
{
	return (letter < 'a') ? Piece::Color::Black : Piece::Color::White;
}

Piece::Type Piece::getType(char letter)
{
	switch (tolower(letter))
	{
		case 'p':
			return Piece::Type::Pawn;
			break;
		case 'r':
			return Piece::Type::Rook;
			break;
		case 'n':
			return Piece::Type::Knight;
			break;
		case 'b':
			return Piece::Type::Bishop;
			break;
		case 'q':
			return Piece::Type::Queen;
			break;
	}

	return Piece::Type::King;
}

std::unique_ptr<Piece> Piece::toPiece(char letter, const Coordinates& coordinates)
{
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

bool Piece::isPiece(char letter)
{
	return letter != 'x';
}

Piece::Type Pawn::getType() const
{
	return Piece::Type::Pawn;
}

Piece::Type Rook::getType() const
{
	return Piece::Type::Rook;
}

Piece::Type Knight::getType() const
{
	return Piece::Type::Knight;
}

Piece::Type Bishop::getType() const
{
	return Piece::Type::Bishop;
}

Piece::Type Queen::getType() const
{
	return Piece::Type::Queen;
}

Piece::Type King::getType() const
{
	return Piece::Type::King;
}

Piece::Traits Pawn::getTraits() const
{
	return { m_color, Piece::Type::Pawn };
}

Piece::Traits Rook::getTraits() const
{
	return { m_color, Piece::Type::Rook };
}

Piece::Traits Knight::getTraits() const
{
	return { m_color, Piece::Type::Knight };
}

Piece::Traits Bishop::getTraits() const
{
	return { m_color, Piece::Type::Bishop };
}

Piece::Traits Queen::getTraits() const
{
	return { m_color, Piece::Type::Queen };
}

Piece::Traits King::getTraits() const
{
	return { m_color, Piece::Type::King };
}

Pawn::Pawn(const Coordinates& coordinates, Color color) : Piece{ coordinates, color } {}
Rook::Rook(const Coordinates& coordinates, Color color) : Piece{ coordinates, color } {}
Knight::Knight(const Coordinates& coordinates, Color color) : Piece{ coordinates, color } {}
Bishop::Bishop(const Coordinates& coordinates, Color color) : Piece{ coordinates, color } {}
Queen::Queen(const Coordinates& coordinates, Color color) : Piece{ coordinates, color } {}
King::King(const Coordinates& coordinates, Color color) : Piece{ coordinates, color } {}