#pragma once
#include "constants.h"
#include <array>

class BoardMatrix
{
	public:

		using Array2d = std::array<char, Constants::array2dSize>;

		BoardMatrix(Array2d& array);
		BoardMatrix(Array2d&& array);
		
		char& operator()(int row, int column);
		const char& operator()(int row, int column) const;

	private:
		
		Array2d m_array{};
};