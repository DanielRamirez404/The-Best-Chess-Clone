#include "chess.h"
#include <iostream>

int main(int, char**)
{
	constexpr int size{ 600 };
	Chess chess(size, "Chess");

	if (chess.hadError())
		std::cout << chess.getError() << '\n';
	
	chess.run();
	
	return 0;
}