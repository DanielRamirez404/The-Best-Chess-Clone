#pragma once
#include <SDL.h>
#include <string_view>

class Game
{
	public:
		Game(int width, int height, std::string_view name);
		Game(int size, std::string_view name) : Game(size, size, name) {};
		virtual void run() const;
		bool hadError() const;
		std::string_view getError() const;
		~Game();
	private:
		mutable SDL_Window* m_window{ nullptr };
		mutable int m_errorCode{ 0 };
};

/*	to-do: code error (enum) and string msg hashmap 

	0 = no error
	1 = SDL initialization error
	2 = window initialization error
 */