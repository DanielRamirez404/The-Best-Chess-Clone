#pragma once
#include "boardMatrix.h"
#include "piece.h"
#include "coordinates.h"
#include <vector>
#include <memory>

class Board
{
	public:

		Board(Piece::Color player);

		char operator()(const Coordinates& coordinates) const;

		std::vector<const Piece*> getPieces();

		void movePieces(const Coordinates& oldCoordinates, const Coordinates& newCoordinates);
		bool isFromPlayer(const Coordinates& coordinates);
		
		//bool isKingMated(Piece::Color color);
		//bool isKingChecked(Piece::Color color);
		
		static bool isOutOfBounds(const Coordinates& coordinates);

	private:

		Piece::Color m_playerColor{};
		BoardMatrix m_matrix{ {} };

		std::vector<std::unique_ptr<Piece>> m_whitePieces{};
		std::vector<std::unique_ptr<Piece>> m_blackPieces{};

		std::vector<std::unique_ptr<Piece>>& getListFromColor(Piece::Color color);
		void erasePieceFromList(const Coordinates& coordinates);
		Piece* getPieceFromList(const Coordinates& coordinates);
		

		//bool isPinned(const std::unique_ptr<Piece>& piece);
		//std::vector<Coordinates> getMoves(const std::unique_ptr<Piece>& piece);
};