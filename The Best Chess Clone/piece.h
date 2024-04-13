#pragma once
#include "coordinates.h"
#include <utility>

struct Piece
{
	enum class Color
	{
		White,
		Black,
	};

	enum class Type
	{
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King,
	};

	Color color{};
	Type type{};
	
	Coordinates coordinates{0, 0};

	Piece() = default;
	Piece(const Coordinates& myCoordinates, Color myColor, Type myType);
};

using PieceTraits = std::pair<Piece::Color, Piece::Type>;