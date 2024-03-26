#include "piece.h"

int Piece::getID() const
{
	return (color * 10) + type;
}

bool PieceComparator::operator()(const Piece& left, const Piece& right) const
{
	return left.getID() < right.getID();
}