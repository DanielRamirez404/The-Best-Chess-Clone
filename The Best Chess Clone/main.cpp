#include "chess.h"
#include <iostream>

int main(int, char**)
{
	Chess chess{};

	if (chess.hadError())
	{
		std::cout << chess.getError() << '\n';
		return 1;
	}

	chess.run();

	return 0;
}

