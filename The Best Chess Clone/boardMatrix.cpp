#include "boardMatrix.h"
#include <array>
#include <utility>

BoardMatrix::BoardMatrix(Array2d& array) : m_array{ array } {}
BoardMatrix::BoardMatrix(Array2d&& array) : m_array{ std::move(array) } {}

char& BoardMatrix::operator()(int row, int column)
{
	return m_array[static_cast<size_t>(row * Constants::squaresPerLine + column )];
}

const char& BoardMatrix::operator()(int row, int column) const
{
	return m_array[static_cast<size_t>(row * Constants::squaresPerLine + column)];
}