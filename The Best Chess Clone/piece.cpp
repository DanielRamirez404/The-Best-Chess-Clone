#include "piece.h"
#include "constants.h"
#include <algorithm>

Piece::Piece(const Coordinates& myCoordinates) : coordinates{ myCoordinates } {}

void Coordinates::toScreenCoord()
{
	x *= Constants::squareSize;
	y *= Constants::squareSize;

	std::swap(x, y);
}

void Coordinates::toMatrixCoord()
{
	for (int i{ 0 }; i <= Constants::squaresPerLine; ++i)
	{
		if (x < i * Constants::squareSize)
		{
			x = i - 1;
			break;
		}
	}

	for (int j{ 0 }; j <= Constants::squaresPerLine; ++j)
	{
		if (y < j * Constants::squareSize)
		{
			y = j - 1;
			break;
		}
	}

	std::swap(x, y);
}

bool operator==(const Coordinates& coor1, const Coordinates& coor2)
{
	return coor1.x == coor1.y && coor2.x == coor2.y;
}

bool operator!=(const Coordinates& coor1, const Coordinates& coor2)
{
	return !(coor1 == coor2);
}