#pragma once
#include <SDL.h>
#include <string_view>
#include <unordered_map>
#include <map>

class Chess
{
	public:

		Chess(const Chess&) = delete;
		void operator=(const Chess&) = delete;
		Chess(int windowSize, std::string_view windowName);
		~Chess();

		void run();
		bool hadError();
		std::string_view getError();
		
	private:

		enum ErrorCode
		{
			None,
			SDL_init,
			Window_init,
			IMG_init,
			IMG_opt,
		};

		struct Piece
		{
			enum Color 
			{
				White,
				Black,
			};

			enum Type
			{
				Pawn,
				Knight,
				Bishop,
				Rook,
				Queen,
				King,
			};

			Color color;
			Type type;
		};

		static std::unordered_map<ErrorCode, std::string_view> errorMap;

		const int m_windowSize{};
		SDL_Window* m_window{ nullptr };
		SDL_Surface* m_screenSurface{ nullptr };
		SDL_Surface* m_boardImg{ nullptr };
		std::map<Piece, SDL_Surface*> pieceImagesMap;
		
		ErrorCode m_errorCode{ ErrorCode::None };

		bool init(std::string_view windowName);
		bool loadResources();
		bool loadBMP(SDL_Surface*& surfacePtr, std::string_view path);
};