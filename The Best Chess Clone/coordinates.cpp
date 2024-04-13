#include "coordinates.h"
#include "piece.h"
#include "constants.h"
#include <algorithm>

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

bool Coordinates::operator==(const Coordinates& coordinates) const
{
	return x == coordinates.x && y == coordinates.y;
}

bool Coordinates::operator!=(const Coordinates& coordinates) const
{
	return !operator==(coordinates);
}

bool Coordinates::operator>(const Coordinates& coordinates) const
{
	return x > coordinates.x || y > coordinates.y;
}

bool Coordinates::operator<(const Coordinates& coordinates) const
{
	return x < coordinates.x || y < coordinates.y;
}

bool Coordinates::operator>=(const Coordinates& coordinates) const
{
	return operator>(coordinates) || operator==(coordinates);
}

bool Coordinates::operator<=(const Coordinates& coordinates) const
{
	return operator<(coordinates) || operator==(coordinates);
}

Coordinates Coordinates::operator+(const Coordinates& coordinates) const
{
	return { x + coordinates.x, y + coordinates.y };
}

Coordinates Coordinates::operator-(const Coordinates& coordinates) const
{
	return { x - coordinates.x, y - coordinates.y };
}

Coordinates Coordinates::operator*(const Coordinates& coordinates) const
{
	return { x * coordinates.x, y * coordinates.y };
}