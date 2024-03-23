#include "chess.h"
#include <SDL.h>
#include <unordered_map>
#include <map>
#include <string_view>
#include <string>

std::unordered_map<Chess::ErrorCode, std::string_view> Chess::errorMap
{
	{ErrorCode::None, "No error"},
	{ErrorCode::SDL_init, "SDL Initialization Error"},
	{ErrorCode::Window_init, "Window Initialization Error"},
	{ErrorCode::IMG_init, "Image Initialization Error"},
	{ErrorCode::IMG_opt, "Image Optimization Error"},
};

Chess::Chess(int windowSize, std::string_view windowName) : m_windowSize{windowSize}
{
	if (!init(windowName))
		return;

	if (!loadResources())
		return;	
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
	SDL_Rect fullBoardRect{0, 0, m_windowSize, m_windowSize};
	SDL_BlitScaled(m_boardImg, nullptr, m_screenSurface, &fullBoardRect);
	SDL_UpdateWindowSurface(m_window);

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

bool Chess::init(std::string_view windowName)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		m_errorCode = ErrorCode::SDL_init;
		return false;
	}

	m_window = SDL_CreateWindow(windowName.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowSize, m_windowSize, SDL_WINDOW_SHOWN);

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
	if (!loadBMP(m_boardImg, "res/board.bmp"))
		return false;

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
			//to-do: use path to load .png
		}
	}

	return true;
}

bool Chess::loadBMP(SDL_Surface*& surfacePtr, std::string_view path)
{
	SDL_Surface* temp = SDL_LoadBMP(path.data());

	if (!temp)
	{
		m_errorCode = ErrorCode::IMG_init;
		return false;
	}

	surfacePtr = SDL_ConvertSurface(temp, m_screenSurface->format, 0);
	SDL_FreeSurface(temp);

	if (!surfacePtr)
	{
		m_errorCode = ErrorCode::IMG_opt;
		return false;
	}

	return true;
}