#pragma once
#include "coordinates.h"
#include <utility>
#include <memory>
#include <vector>

class Board;

class Piece
{
	public:

		enum class Color
		{
			White,
			Black,
		};

		enum class Type
		{
			Pawn,
			Knight,
			Bishop,
			Rook,
			Queen,
			King,
		};

		using Traits = std::pair<Piece::Color, Piece::Type>;

		Color getColor() const;
		const Coordinates& getCoordinates() const;
		Coordinates& getCoordinates();
		bool isSameColorPiece(char letter) const;
		bool isPinned(Board& board);
		bool hasMoved() const;
		void addMovedFlag() const;

		virtual Type getType() const = 0;
		virtual Traits getTraits() const = 0;
		virtual std::vector<Coordinates> getAttacks(const Board& board) = 0;
		virtual std::vector<Coordinates> getMoves(Board& board) = 0;
		virtual int getValue() const = 0;
		virtual char getLetter() const = 0;

		static Color getColor(char letter);
		static Type getType(char letter);
		static std::unique_ptr<Piece> toPiece(char letter, const Coordinates& coordinates, bool hasMoved);
		static bool isPiece(char letter);
		static void setPlayerColor(Color color);
		static Color getPlayerColor();
		static int getForwardDirection(Color color);

	protected:

		Piece(const Coordinates& coordinates, Color color, bool hasMoved);

		Color m_color{};
		Coordinates m_coordinates{};
		mutable bool m_hasMoved{ false };

		static Color s_playerColor;
};

class Pawn : public Piece
{
	public:
		Pawn(const Coordinates& coordinates, Color color, bool hasMoved);
		Type getType() const override;
		Traits getTraits() const override;
		int getValue() const override;
		char getLetter() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
		std::vector<Coordinates> getMoves(Board& board) override;
};

class Rook : public Piece
{
	public:
		Rook(const Coordinates& coordinates, Color color, bool hasMoved);
		Type getType() const override;
		Traits getTraits() const override;
		int getValue() const override;
		char getLetter() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
		std::vector<Coordinates> getMoves(Board& board) override;
};

class Knight : public Piece
{
	public:
		Knight(const Coordinates& coordinates, Color color, bool hasMoved);
		Type getType() const override;
		Traits getTraits() const override;
		int getValue() const override;
		char getLetter() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
		std::vector<Coordinates> getMoves(Board& board) override;
};

class Bishop : public Piece
{
	public:
		Bishop(const Coordinates& coordinates, Color color, bool hasMoved);
		Type getType() const override;
		Traits getTraits() const override;
		int getValue() const override;
		char getLetter() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
		std::vector<Coordinates> getMoves(Board& board) override;
};

class Queen : public Piece
{
	public:
		Queen(const Coordinates& coordinates, Color color, bool hasMoved);
		Type getType() const override;
		Traits getTraits() const override;
		int getValue() const override;
		char getLetter() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
		std::vector<Coordinates> getMoves(Board& board) override;
};

class King : public Piece
{
	public:
		King(const Coordinates& coordinates, Color color, bool hasMoved);
		Type getType() const override;
		Traits getTraits() const override;
		int getValue() const override;
		char getLetter() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
		std::vector<Coordinates> getMoves(Board& board) override;
};

Piece::Color operator!(Piece::Color color);