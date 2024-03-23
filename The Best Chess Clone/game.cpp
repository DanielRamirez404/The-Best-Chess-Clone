#include "game.h"
#include <SDL.h>
#include <string_view>

Game::Game(int width, int height, std::string_view name)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		m_errorCode = 1;
		return;
	}

	m_window = SDL_CreateWindow(name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	
	if (!m_window)
		m_errorCode = 2;
}

Game::~Game()
{
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	SDL_Quit();
}

void Game::run() const
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

bool Game::hadError() const
{
	return m_errorCode;
}

std::string_view Game::getError() const
{
	switch (m_errorCode)
	{
		case 1:
			return "SDL Initialization Error";
			break;
		case 2:
			return "Window Initialization Error";
			break;
	}

	return "No errors";
}