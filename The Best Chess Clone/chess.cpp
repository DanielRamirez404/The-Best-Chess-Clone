#include "chess.h"
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string_view>
#include <string>

std::unordered_map<Chess::ErrorCode, std::string_view> Chess::m_errorMap
{
	{ ErrorCode::None, "No error" },
	{ ErrorCode::SDL_init, "SDL Initialization Error" },
	{ ErrorCode::SDL_IMG_init, "SDL Image Initialization Error" },
	{ ErrorCode::Window_init, "Window Initialization Error" },
	{ ErrorCode::Render_init, "Renderer Initialization Error" },
	{ ErrorCode::IMG_load, "Image Loading Error" },
	{ ErrorCode::Texture_load, "Texture Loading Error" },
};

Chess::Chess(int windowSize, std::string_view windowName) : m_windowSize{ windowSize }, m_squareSize{ m_windowSize / m_squaresPerLine }
{
	if (!init(windowName))
		return;

	if (!loadResources())
		return;	
}

Chess::~Chess()
{
	for (auto& pair : m_pieceTextureMap)
	{
		SDL_DestroyTexture(pair.second);
		pair.second = nullptr;
	}

	SDL_DestroyTexture(m_boardTexture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	IMG_Quit();
	SDL_Quit();

	m_renderer = nullptr;
	m_window = nullptr;
	m_boardTexture = nullptr;
}

void Chess::run()
{
	SDL_Texture*& piece = m_pieceTextureMap[{Piece::Color::White, Piece::Type::Queen}];

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(piece, SDL_BLENDMODE_BLEND);

	SDL_Rect fullBoardRect{ 0, 0, m_windowSize, m_windowSize };
	SDL_Rect singleSquareRect{ 0, 0, m_squareSize, m_squareSize };

	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_boardTexture, nullptr, &fullBoardRect);
	SDL_RenderCopy(m_renderer, piece, nullptr, &singleSquareRect);
	SDL_RenderPresent(m_renderer);

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
	return m_errorMap[m_errorCode];
}

bool Chess::init(std::string_view windowName)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		m_errorCode = ErrorCode::SDL_init;
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		m_errorCode = ErrorCode::SDL_IMG_init;
		return false;
	}

	m_window = SDL_CreateWindow(windowName.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowSize, m_windowSize, SDL_WINDOW_SHOWN);

	if (!m_window)
	{
		m_errorCode = ErrorCode::Window_init;
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if (!m_renderer)
	{
		m_errorCode = ErrorCode::Render_init;
		return false;
	}

	return true;
}

bool Chess::loadResources()
{
	if (!loadTexture(m_boardTexture, "res/board.bmp"))
		return false;

	const std::map<Piece::Color, std::string_view> colorMap
	{
		{ Piece::Color::White, "white" },
		{ Piece::Color::Black, "black" },
	};

	const std::map<Piece::Type, std::string_view> typeMap
	{
		{ Piece::Type::Pawn, "pawn" },
		{ Piece::Type::Knight, "knight" },
		{ Piece::Type::Bishop, "bishop" },
		{ Piece::Type::Rook, "rook" },
		{ Piece::Type::Queen, "queen" },
		{ Piece::Type::King, "king" },
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

			SDL_Texture* pieceImg{ nullptr };

			if (!loadTexture(pieceImg, path))
				return false;
				
			m_pieceTextureMap[{ color, type }] = std::move(pieceImg);
		}
	}

	return true;
}

bool Chess::loadTexture(SDL_Texture*& texturePtr, std::string_view path)
{
	SDL_Surface* temp = (path.find(".bmp") != std::string_view::npos) ? SDL_LoadBMP(path.data()) : IMG_Load(path.data());

	if (!temp)
	{
		m_errorCode = ErrorCode::IMG_load;
		return false;
	}

	texturePtr = SDL_CreateTextureFromSurface(m_renderer, temp);
	SDL_FreeSurface(temp);

	if (!texturePtr)
	{
		m_errorCode = ErrorCode::Texture_load;
		return false;
	}

	return true;
}

int Chess::Piece::getID() const
{
	return (color * 10) + type;
}

bool Chess::PieceComparator::operator()(const Chess::Piece& left, const Chess::Piece& right) const
{
	return left.getID() < right.getID();
}