#pragma once

struct Piece
{
	enum Color
	{
		White,
		Black,
	};

	enum Type
	{
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King,
	};

	Color color;
	Type type;
	int coorX{ 0 };
	int coorY{ 0 };

	int getID() const;
};

struct PieceComparator
{
	bool operator()(const Piece& left, const Piece& right) const;
};