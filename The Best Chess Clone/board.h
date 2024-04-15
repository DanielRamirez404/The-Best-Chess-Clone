#pragma once
#include "boardMatrix.h"
#include "piece.h"
#include "coordinates.h"
#include <vector>
#include <memory>

class Board
{
	public:

		Board();

		std::vector<const Piece*> getPieces();
		char at(const Coordinates& coordinates);
		void movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates);
		bool isMovable(const Coordinates& coordinates);
		//bool isKingMated(Piece::Color color);
		//bool isKingChecked(Piece::Color color);

		std::vector<Coordinates> getAttacks(const std::unique_ptr<Piece>& piece);

	private:

		BoardMatrix m_matrix{ {} };
		std::vector<std::unique_ptr<Piece>> m_whitePieces{};
		std::vector<std::unique_ptr<Piece>> m_blackPieces{};

		std::vector<std::unique_ptr<Piece>>& getListFromColor(Piece::Color color);
		static bool isOutOfBounds(const Coordinates& coordinates);

		//bool isPinned(const std::unique_ptr<Piece>& piece);
		//std::vector<Coordinates> getMoves(const std::unique_ptr<Piece>& piece);
};