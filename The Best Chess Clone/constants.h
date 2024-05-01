#pragma once
#include <string_view>
#include <limits>

namespace Constants
{
	inline constexpr std::string_view title{ "Chess" };
	inline constexpr int windowSize{ 600 };
	inline constexpr int squaresPerLine{ 8 };
	inline constexpr int squareSize{ windowSize / squaresPerLine };
	inline constexpr int array2dSize{ squaresPerLine * squaresPerLine };
	inline constexpr int piecesPerColor{ squaresPerLine * 2 };
	inline constexpr int maxEval{ std::numeric_limits<int>::max() / 2 }; //big number but not close enough to the limits to mess up something
	inline constexpr int minEval{ -maxEval };							 //must be equal as maxEval * -1
}