#include "chess.h"
#include "piece.h"
#include "coordinates.h"
#include "constants.h"
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string_view>
#include <string>
#include <vector>

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

Chess::Chess()
{
	m_errorCode = init();

	if (!hadError())
		m_errorCode = loadResources();
}

#include <iostream>

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
	renderBoard();

	Coordinates oldCoordinates{};
	bool isClickToMove{ false };

	SDL_Event event{};
	while (true)
	{
		while (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return;

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (isClickToMove)
					{
						isClickToMove = false;

						Coordinates newCoordinates{ event.button.x, event.button.y };
						newCoordinates.toMatrixCoord();

						auto moves{ m_board.getMoves(oldCoordinates) };

						if (std::find(moves.begin(), moves.end(), newCoordinates) == moves.end())
						{
							renderBoard();
							continue;
						}

						if (oldCoordinates != newCoordinates)
							m_board.makeMove(oldCoordinates, newCoordinates);

						renderBoard();
						m_board.makeAIMove();
						renderBoard();
					}
					else
					{
						oldCoordinates = { event.button.x, event.button.y };
						oldCoordinates.toMatrixCoord();

						if (m_board.isFromPlayer(oldCoordinates))
						{
							isClickToMove = true;
							auto moves{ m_board.getMoves(oldCoordinates) };
							renderBoard(moves);
						}
					}
				}
			}
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

Chess::ErrorCode Chess::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return ErrorCode::SDL_init;

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return ErrorCode::SDL_IMG_init;

	m_window = SDL_CreateWindow(Constants::title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Constants::windowSize, Constants::windowSize, SDL_WINDOW_SHOWN);

	if (!m_window)
		return ErrorCode::Window_init;

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if (!m_renderer)
		return ErrorCode::Render_init;

	return ErrorCode::None;
}

Chess::ErrorCode Chess::loadResources()
{
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	
	if (!loadTexture(m_boardTexture, "res/board.bmp"))
		return ErrorCode::Texture_load;

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

			SDL_Texture* texture{ nullptr };

			if (!loadTexture(texture, path))
				return ErrorCode::Texture_load;
			
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			m_pieceTextureMap[ { color, type } ] = std::move(texture);
		}
	}

	return ErrorCode::None;
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

void Chess::renderBoard()
{
	SDL_RenderClear(m_renderer);

	SDL_Rect fullBoardRect{ 0, 0, Constants::windowSize, Constants::windowSize };
	SDL_RenderCopy(m_renderer, m_boardTexture, nullptr, &fullBoardRect);

	for (auto& piece : m_board.getPieces())
	{
		Coordinates screenCoordinates{ piece->getCoordinates() };
		screenCoordinates.toScreenCoord();
		SDL_Rect squareRect{ screenCoordinates.x,screenCoordinates.y, Constants::squareSize, Constants::squareSize };
		SDL_Texture*& pieceTexture{ m_pieceTextureMap[ piece->getTraits() ]};
		SDL_RenderCopy(m_renderer, pieceTexture, nullptr, &squareRect);
	}

	SDL_RenderPresent(m_renderer);
}

void Chess::renderBoard(std::vector<Coordinates>& attacks)
{
	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 150);

	SDL_Rect fullBoardRect{ 0, 0, Constants::windowSize, Constants::windowSize };
	SDL_RenderCopy(m_renderer, m_boardTexture, nullptr, &fullBoardRect);

	for (auto& attack : attacks)
	{
		attack.toScreenCoord();
		SDL_Rect squareRect{ attack.x, attack.y, Constants::squareSize, Constants::squareSize };
		SDL_RenderFillRect(m_renderer, &squareRect);
	}

	for (auto& piece : m_board.getPieces())
	{
		Coordinates screenCoordinates{ piece->getCoordinates() };
		screenCoordinates.toScreenCoord();
		SDL_Rect squareRect{ screenCoordinates.x,screenCoordinates.y, Constants::squareSize, Constants::squareSize };
		SDL_Texture*& pieceTexture{ m_pieceTextureMap[{ piece->getColor(), piece->getType() }]};
		SDL_RenderCopy(m_renderer, pieceTexture, nullptr, &squareRect);
	}

	SDL_RenderPresent(m_renderer);
}