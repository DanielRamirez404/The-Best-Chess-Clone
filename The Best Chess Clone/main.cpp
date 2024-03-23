#include "game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int, char**)
{
	constexpr int size{ 600 };
	const Game game(size, "Chess");
	if (game.hadError())
		std::cout << game.getError() << '\n';
	game.run();
	return 0;
}