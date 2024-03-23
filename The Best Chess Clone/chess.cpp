#include "chess.h"
#include <SDL.h>
#include <string_view>

std::unordered_map<Chess::ErrorCode, std::string_view> Chess::errorMap
{
	{ErrorCode::None, "No error"},
	{ErrorCode::SDL_init, "SDL Initialization Error"},
	{ErrorCode::Window_init, "Window Initialization Error"},
};

Chess::Chess(int width, int height, std::string_view name)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		m_errorCode = ErrorCode::SDL_init;
		return;
	}

	m_window = SDL_CreateWindow(name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	
	if (!m_window)
		m_errorCode = ErrorCode::Window_init;
}

Chess::~Chess()
{
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	SDL_Quit();
}

void Chess::run()
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

bool Chess::hadError()
{
	return m_errorCode != ErrorCode::None;
}

std::string_view Chess::getError()
{
	return errorMap[m_errorCode];
}