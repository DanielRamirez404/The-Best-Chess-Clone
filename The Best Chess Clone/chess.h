#pragma once
#include "piece.h"
#include "board.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string_view>
#include <unordered_map>
#include <map>
#include <vector>

class Chess
{
	public:

		Chess();
		~Chess();

		void run();
		bool hadError();
		std::string_view getError();
		
	private:

		enum ErrorCode
		{
			None,
			SDL_init,
			SDL_IMG_init,
			Window_init,
			Render_init,
			IMG_load,
			Texture_load,
		};

		static std::unordered_map<ErrorCode, std::string_view> m_errorMap;

		SDL_Window* m_window{ nullptr };
		SDL_Renderer* m_renderer{ nullptr };
		SDL_Texture* m_boardTexture{ nullptr };
		std::map<PieceTraits, SDL_Texture*> m_pieceTextureMap;
		
		ErrorCode m_errorCode{};

		Board m_board{};

		Chess(const Chess&) = delete;
		void operator=(const Chess&) = delete;

		ErrorCode init();
		ErrorCode loadResources();
		bool loadTexture(SDL_Texture*& texturePtr, std::string_view path);
		void renderBoard();
		void renderBoard(std::vector<Coordinates>&& attacks);
};