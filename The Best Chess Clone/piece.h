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

		virtual Type getType() const = 0;
		virtual Traits getTraits() const = 0;
		virtual std::vector<Coordinates> getAttacks(const Board& board) = 0;

		static Color getColor(char letter);
		static Type getType(char letter);
		static std::unique_ptr<Piece> toPiece(char letter, const Coordinates& coordinates);
		static bool isPiece(char letter);

	protected:

		Piece(const Coordinates& coordinates, Color color);

		Color m_color{};
		Coordinates m_coordinates{};
};

class Pawn : public Piece
{
	public:
		Pawn(const Coordinates& coordinates, Color color);
		Type getType() const override;
		Traits getTraits() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
};

class Rook : public Piece
{
	public:
		Rook(const Coordinates& coordinates, Color color);
		Type getType() const override;
		Traits getTraits() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
};

class Knight : public Piece
{
	public:
		Knight(const Coordinates& coordinates, Color color);
		Type getType() const override;
		Traits getTraits() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
};

class Bishop : public Piece
{
	public:
		Bishop(const Coordinates& coordinates, Color color);
		Type getType() const override;
		Traits getTraits() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
};

class Queen : public Piece
{
	public:
		Queen(const Coordinates& coordinates, Color color);
		Type getType() const override;
		Traits getTraits() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
};

class King : public Piece
{
	public:
		King(const Coordinates& coordinates, Color color);
		Type getType() const override;
		Traits getTraits() const override;
		std::vector<Coordinates> getAttacks(const Board& board) override;
};