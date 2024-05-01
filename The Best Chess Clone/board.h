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

		bool isFromPlayer(const Coordinates& coordinates) const;
		bool isLegalMove(const Coordinates& oldCoordinates, const Coordinates& newCoordinates) const;
		bool isAttacked(const Coordinates& coordinates) const;
		
		bool isKingMated(Piece::Color color);
		bool isKingChecked(Piece::Color color) const;

		Coordinates generateAIMove();
		
		static bool isOutOfBounds(const Coordinates& coordinates);

		friend class Piece;

	private:

		struct EvaluatedMove 
		{
			Coordinates move{ -1, -1 };		//starts with impossible move
			int eval{ -1000000 };			//and min eval
		};

		Piece::Color m_playerColor{};
		mutable BoardMatrix m_matrix{ {} };

		std::vector<std::unique_ptr<Piece>> m_whitePieces{};
		std::vector<std::unique_ptr<Piece>> m_blackPieces{};

		const std::vector<std::unique_ptr<Piece>>& getListFromColor(Piece::Color color) const;
		std::vector<std::unique_ptr<Piece>>& getListFromColor(Piece::Color color);
		void erasePieceFromList(const Coordinates& coordinates);
		Piece* getPieceFromList(const Coordinates& coordinates);

		EvaluatedMove& max(EvaluatedMove& firstMove, EvaluatedMove& secondMove);
		EvaluatedMove getBestMoveForColor(Piece::Color color, int deepness);
		int getColorEval(Piece::Color color);
};