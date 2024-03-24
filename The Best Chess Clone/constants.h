#pragma once
#include <SDL.h>
#include <string_view>

namespace Constants
{
	inline constexpr std::string_view title{ "Chess" };
	inline constexpr int windowSize{ 600 };
	inline constexpr int squaresPerLine{ 8 };
	inline constexpr int squareSize{ windowSize / squaresPerLine };
}