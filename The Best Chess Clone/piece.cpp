#include "piece.h"
#include "coordinates.h"
#include <utility>

Piece::Piece(const Coordinates& coordinates, Color color)
	: m_coordinates{ coordinates }, m_color{ color } {}

const Piece::Color& Piece::getColor() const
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