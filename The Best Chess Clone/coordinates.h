#pragma once

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