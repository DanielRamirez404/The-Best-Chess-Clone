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

		std::vector<std::unique_ptr<Piece>> getPieces();
		void movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates);
		bool isMovable(const Coordinates& coordinates);
		bool isKingMated(Piece::Color color);
		bool isKingChecked(Piece::Color color);

		std::unique_ptr<Piece> getPiece(int i, int j);
		std::unique_ptr<Piece> getPiece(const Coordinates& coordinates);

		std::vector<Coordinates> getAttacks(const std::unique_ptr<Piece>& piece);

	private:

		BoardMatrix m_matrix{ {} };
		std::vector<std::unique_ptr<Piece>> m_whitePieces{};
		std::vector<std::unique_ptr<Piece>> m_blackPieces{};

		bool isOutOfBounds(const Coordinates& coordinates);

		Piece::Type getType(char letter);
		Piece::Color getColor(char letter);

		//bool isPinned(const std::unique_ptr<Piece>& piece);
		//std::vector<Coordinates> getMoves(const std::unique_ptr<Piece>& piece);
};

/*
	todo: static check if it's out of bounds

*/