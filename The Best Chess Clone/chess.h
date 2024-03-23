#pragma once
#include <SDL.h>
#include <string_view>
#include <unordered_map>

class Chess
{
	public:

		Chess(const Chess&) = delete;
		void operator=(const Chess&) = delete;
		Chess(int width, int height, std::string_view name);
		Chess(int size, std::string_view name) : Chess(size, size, name) {};
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
		};

		static std::unordered_map<Chess::ErrorCode, std::string_view> errorMap;

		SDL_Window* m_window{ nullptr };
		ErrorCode m_errorCode{ ErrorCode::None };
};