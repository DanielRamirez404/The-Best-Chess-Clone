#include "chess.h"
#include <iostream>
#include <cstdlib>

int main(int, char**)
{
	srand(time(0));

	Chess chess{};

	if (chess.hadError())
	{
		std::cout << chess.getError() << '\n';
		return 1;
	}

	chess.run();

	return 0;
}

