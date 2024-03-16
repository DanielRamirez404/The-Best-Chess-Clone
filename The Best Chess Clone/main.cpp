#include <SDL.h>
#include <iostream>

void run(SDL_Window* window);

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	constexpr int windowSize{ 400 };
	SDL_Window* window{ SDL_CreateWindow("Hello Github!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize, windowSize, SDL_WINDOW_SHOWN) };

	if (!window)
		return 1;

	run(window);

	SDL_DestroyWindow(window);

	return 0;
}

void run(SDL_Window* window)
{
	SDL_Event event{};
	while (true)
	{
		while (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return;
		}
	}
}