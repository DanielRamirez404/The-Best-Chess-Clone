#include "chess.h"
#include <SDL.h>
#include <unordered_map>
#include <map>
#include <string_view>
#include <string>
#include <iostream>

std::unordered_map<Chess::ErrorCode, std::string_view> Chess::errorMap
{
	{ErrorCode::None, "No error"},
	{ErrorCode::SDL_init, "SDL Initialization Error"},
	{ErrorCode::Window_init, "Window Initialization Error"},
	{ErrorCode::IMG_init, "Image Initialization Error"},
};

Chess::Chess(int windowSize, std::string_view windowName)
{
	if (!init(windowSize, windowName))
		return;

	if (!loadResources())
		return;

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = windowSize;
	rect.h = windowSize;
	SDL_BlitScaled(m_boardImg, NULL, m_screenSurface, &rect);
	SDL_UpdateWindowSurface(m_window);
}

Chess::~Chess()
{
	SDL_FreeSurface(m_boardImg);
	m_boardImg = nullptr;
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	m_screenSurface = nullptr;
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

bool Chess::init(int windowSize, std::string_view windowName)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		m_errorCode = ErrorCode::SDL_init;
		return false;
	}

	m_window = SDL_CreateWindow(windowName.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize, windowSize, SDL_WINDOW_SHOWN);

	if (!m_window)
	{
		m_errorCode = ErrorCode::Window_init;
		return false;
	}

	m_screenSurface = SDL_GetWindowSurface(m_window);

	return true;
}

bool Chess::loadResources()
{
	SDL_Surface* temp = SDL_LoadBMP("res/board.bmp");

	if (!temp)
	{
		m_errorCode = ErrorCode::IMG_init;
		return false;
	}

	m_boardImg = SDL_ConvertSurface(temp, m_screenSurface->format, 0);

	if (!m_boardImg)
	{
		m_errorCode = ErrorCode::IMG_init;
		return false;
	}

	SDL_FreeSurface(temp);
	temp = nullptr;

	const std::map<Piece::Color, std::string_view> colorMap
	{
		{Piece::Color::White, "white"},
		{Piece::Color::Black, "black"},
	};

	const std::map<Piece::Type, std::string_view> typeMap
	{
		{Piece::Type::Pawn, "pawn"},
		{Piece::Type::Knight, "knight"},
		{Piece::Type::Bishop, "bishop"},
		{Piece::Type::Rook, "rook"},
		{Piece::Type::Queen, "queen"},
		{Piece::Type::King, "king"},
	};

	for (const auto& [color, colorString] : colorMap)
	{
		for (const auto& [type, typeString] : typeMap)
		{
			std::string path{ "res/" };
			path.append(colorString);
			path += '_';
			path.append(typeString);
			path.append(".png");
			std::cout << path << '\n';
		}
		std::cout << '\n';
	}

	return true;
}