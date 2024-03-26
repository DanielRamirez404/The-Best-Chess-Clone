#pragma once
#include "constants.h"
#include <array>
#include <functional>

class BoardMatrix
{
	public:

		using Array2d = std::array<char, Constants::array2dSize>;
		
		char operator()(int row, int column) const { return m_array.get()[static_cast<size_t>(Constants::array2dSize)] };

	private:
		
		std::reference_wrapper<Array2d> m_array;
};