#pragma once
#include <utility>

struct Coordinates
{
	int x{};
	int y{};
	void toMatrixCoord();
	void toScreenCoord();
	bool operator==(const Coordinates& coordinates) const;
	bool operator!=(const Coordinates& coordinates) const;
	bool operator>(const Coordinates& coordinates) const;
	bool operator<(const Coordinates& coordinates) const;
	bool operator>=(const Coordinates& coordinates) const;
	bool operator<=(const Coordinates& coordinates) const;
	Coordinates operator+(const Coordinates& coordinates) const;
	Coordinates operator-(const Coordinates& coordinates) const;
	Coordinates operator*(const Coordinates& coordinates) const;
};

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