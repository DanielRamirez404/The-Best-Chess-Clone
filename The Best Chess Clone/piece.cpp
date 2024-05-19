#include "piece.h"
#include "coordinates.h"
#include "constants.h"
#include "board.h"
#include <memory>
#include <cctype>
#include <algorithm>
#include <vector>

Piece::Color Piece::s_playerColor{ Piece::Color::White };

Piece::Piece(const Coordinates& coordinates, Color color, bool hasMoved)
	: m_coordinates{ coordinates }, m_color{ color }, m_hasMoved{ hasMoved } {}

Piece::Color operator!(Piece::Color color)
{
	return (color == Piece::Color::White) ? Piece::Color::Black : Piece::Color::White;
}

Piece::Color Piece::getColor() const
{
	return m_color;
}

const Coordinates& Piece::getCoordinates() const
{
	return m_coordinates;
}

Coordinates& Piece::getCoordinates()
{
	return m_coordinates;
}

Piece::Color Piece::getColor(char letter)
{
	return (letter < 'a') ? Piece::Color::Black : Piece::Color::White;
}

bool Piece::isSameColorPiece(char letter) const 
{
	return isPiece(letter) && m_color == getColor(letter);
}

Piece::Type Piece::getType(char letter)
{
	switch (tolower(letter))
	{
		case 'p':
			return Piece::Type::Pawn;
			break;
		case 'r':
			return Piece::Type::Rook;
			break;
		case 'n':
			return Piece::Type::Knight;
			break;
		case 'b':
			return Piece::Type::Bishop;
			break;
		case 'q':
			return Piece::Type::Queen;
			break;
	}

	return Piece::Type::King;
}

std::unique_ptr<Piece> Piece::toPiece(char letter, const Coordinates& coordinates, bool hasMoved)
{
	switch (getType(letter))
	{
		case Piece::Type::Pawn:
			return std::make_unique<Pawn>(coordinates, getColor(letter), hasMoved);
			break;
		case Piece::Type::Rook:
			return std::make_unique<Rook>(coordinates, getColor(letter), hasMoved);
			break;
		case Piece::Type::Knight:
			return std::make_unique<Knight>(coordinates, getColor(letter), hasMoved);
			break;
		case Piece::Type::Bishop:
			return std::make_unique<Bishop>(coordinates, getColor(letter), hasMoved);
			break;
		case Piece::Type::Queen:
			return std::make_unique<Queen>(coordinates, getColor(letter), hasMoved);
			break;
		case Piece::Type::King:
			return std::make_unique<King>(coordinates, getColor(letter), hasMoved);
			break;
	}

	return nullptr;
}

bool Piece::isPiece(char letter)
{
	return letter != 'x';
}

void Piece::setPlayerColor(Piece::Color color)
{
	s_playerColor = color;
}

Piece::Color Piece::getPlayerColor()
{
	return s_playerColor;
}

int Piece::getForwardDirection(Piece::Color color)
{
	return (s_playerColor == color) ? -1 : 1;
}

bool Piece::isPinned(Board& board)
{
	char& boardPosition{ board.m_matrix(m_coordinates) };
	const char letter{ boardPosition };

	boardPosition = 'x';
	bool isCheckWithoutPiece{ board.isKingChecked(m_color) };
	boardPosition = letter;

	return isCheckWithoutPiece;
}

bool Piece::hasMoved() const
{
	return m_hasMoved;
}

void Piece::addMovedFlag() const
{
	m_hasMoved = true;
}

Piece::Type Pawn::getType() const
{
	return Piece::Type::Pawn;
}

Piece::Type Rook::getType() const
{
	return Piece::Type::Rook;
}

Piece::Type Knight::getType() const
{
	return Piece::Type::Knight;
}

Piece::Type Bishop::getType() const
{
	return Piece::Type::Bishop;
}

Piece::Type Queen::getType() const
{
	return Piece::Type::Queen;
}

Piece::Type King::getType() const
{
	return Piece::Type::King;
}

Piece::Traits Pawn::getTraits() const
{
	return { m_color, Piece::Type::Pawn };
}

Piece::Traits Rook::getTraits() const
{
	return { m_color, Piece::Type::Rook };
}

Piece::Traits Knight::getTraits() const
{
	return { m_color, Piece::Type::Knight };
}

Piece::Traits Bishop::getTraits() const
{
	return { m_color, Piece::Type::Bishop };
}

Piece::Traits Queen::getTraits() const
{
	return { m_color, Piece::Type::Queen };
}

Piece::Traits King::getTraits() const
{
	return { m_color, Piece::Type::King };
}

std::vector<Coordinates> Pawn::getAttacks(const Board& board)
{
	std::vector<Coordinates> attacks{};

	Coordinates rightCapture{ m_coordinates + Coordinates{ getForwardDirection(m_color), 1 } };
	Coordinates leftCapture{ m_coordinates + Coordinates{ getForwardDirection(m_color), -1 } };

	if (!Board::isOutOfBounds(rightCapture))
	{
		const char letter{ board(rightCapture) };
		if (!isSameColorPiece(letter))
			attacks.push_back(std::move(rightCapture));
	}

	if (!Board::isOutOfBounds(leftCapture))
	{
		const char letter{ board(leftCapture) };
		if (!isSameColorPiece(letter))
			attacks.push_back(std::move(leftCapture));
	}
	
	return attacks;
}

std::vector<Coordinates> Rook::getAttacks(const Board& board)
{
	std::vector<Coordinates> attacks{};

	constexpr std::array<Coordinates, 4> directions{ { {0, 1}, {0, -1}, {1, 0}, {-1, 0} } };

	for (size_t i{ 0 }; i < directions.size(); ++i)
	{
		for (int j{ 1 }; j < Constants::squaresPerLine; ++j)
		{
			Coordinates current{ m_coordinates + Coordinates{ j, j } * directions[i] };

			if (Board::isOutOfBounds(current))
				break;

			char letter{ board(current) };

			if (!isSameColorPiece(letter))
				attacks.push_back(std::move(current));

			if (isPiece(letter))
				break;
		}
	}

	return attacks;
}

std::vector<Coordinates> Knight::getAttacks(const Board& board)
{
	std::vector<Coordinates> attacks{};

	constexpr std::array<Coordinates, 8> directions{ { {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2} } };

	for (size_t i{ 0 }; i < directions.size(); ++i)
	{
		Coordinates current{ m_coordinates + directions[i] };

		if (Board::isOutOfBounds(current) || isSameColorPiece(board(current)))
			continue;

		attacks.push_back(std::move(current));
	}

	return attacks;
}

std::vector<Coordinates> Bishop::getAttacks(const Board& board)
{
	std::vector<Coordinates> attacks{};

	constexpr std::array<Coordinates, 4> directions{ { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} } };

	for (size_t i{ 0 }; i < directions.size(); ++i)
	{
		for (int j{ 1 }; j < Constants::squaresPerLine; ++j)
		{
			Coordinates current{ m_coordinates + Coordinates{ j, j } *directions[i] };

			if (Board::isOutOfBounds(current))
				break;

			char letter{ board(current) };

			if (!isSameColorPiece(letter))
				attacks.push_back(std::move(current));

			if (isPiece(letter))
				break;
		}
	}

	return attacks;
}

std::vector<Coordinates> Queen::getAttacks(const Board& board)
{
	std::vector<Coordinates> attacks{ Rook{m_coordinates, m_color, true}.getAttacks(board) };

	for (auto& attack : Bishop{ m_coordinates, m_color, true}.getAttacks(board))
		attacks.push_back(std::move(attack));

	return attacks;
}

std::vector<Coordinates> King::getAttacks(const Board& board)
{
	std::vector<Coordinates> attacks{};

	constexpr std::array<Coordinates, 8> directions{ { {0, 1}, {0, -1}, {1, 0}, {-1, 0}, { 1, 1 }, {-1, -1}, {-1, 1}, {1, -1} } };

	for (size_t i{ 0 }; i < directions.size(); ++i)
	{
		Coordinates current{ m_coordinates + directions[i] };

		if (Board::isOutOfBounds(current))
			continue;

		char letter{ board(current) };

		if (!isSameColorPiece(letter))
			attacks.push_back(std::move(current));
	}

	return attacks;
}

std::vector<Coordinates> Pawn::getMoves(Board& board)
{
	std::vector<Coordinates> moves{};

	Coordinates moveForward{ m_coordinates + Coordinates{ getForwardDirection(m_color), 0 } };

	if (!board.isOutOfBounds(moveForward) && !isPiece(board(moveForward)))
	{
		moves.push_back(std::move(moveForward));

		if (!m_hasMoved)
		{
			Coordinates moveTwiceForward{ m_coordinates + Coordinates{ getForwardDirection(m_color) * 2, 0 } };
			if (!board.isOutOfBounds(moveTwiceForward) && !isPiece(board(moveTwiceForward)))
				moves.push_back(std::move(moveTwiceForward));
		}
	}
		
	for (auto& attack : getAttacks(board))
		if (isPiece(board(attack)) || board.isEnPassant(attack, m_color))
			moves.push_back(std::move(attack));
	
	if (board.isKingChecked(m_color) || isPinned(board))
		std::erase_if(moves, [&](const Coordinates& move) { return !board.isLegalMove(m_coordinates, move); });

	return moves;
}

std::vector<Coordinates> Rook::getMoves(Board& board)
{
	//todo: add castle

	std::vector<Coordinates> moves{ getAttacks(board) };

	if (board.isKingChecked(m_color) || isPinned(board))
		std::erase_if(moves, [&](const Coordinates& move) { return !board.isLegalMove(m_coordinates, move); });

	return moves;
}

std::vector<Coordinates> Knight::getMoves(Board& board)
{
	return (board.isKingChecked(m_color) || isPinned(board)) ? std::vector<Coordinates>{} : getAttacks(board);
}

std::vector<Coordinates> Bishop::getMoves(Board& board)
{
	std::vector<Coordinates> moves{ getAttacks(board) };

	if (board.isKingChecked(m_color) || isPinned(board))
		std::erase_if(moves, [&](const Coordinates& move) { return !board.isLegalMove(m_coordinates, move); });

	return moves;
}

std::vector<Coordinates> Queen::getMoves(Board& board)
{
	std::vector<Coordinates> moves{ getAttacks(board) };

	if (board.isKingChecked(m_color) || isPinned(board))
		std::erase_if(moves, [&](const Coordinates& move) { return !board.isLegalMove(m_coordinates, move); });

	return moves;
}

std::vector<Coordinates> King::getMoves(Board& board)
{
	//todo: add castle
	std::vector<Coordinates> moves{ getAttacks(board) };
	std::erase_if(moves, [&](const Coordinates& move) { return !board.isLegalMove(m_coordinates, move); });
	return moves;
}

int Pawn::getValue() const
{
	return 1;
}

int Knight::getValue() const
{
	return 3;
}

int Bishop::getValue() const
{
	return 3;
}

int Rook::getValue() const
{
	return 5;
}

int Queen::getValue() const
{
	return 9;
}

int King::getValue() const
{
	return 0;	//not used directly in evaluations
}

char Pawn::getLetter() const
{
	return (m_color == Piece::Color::White) ? 'p' : 'P';
}

char King::getLetter() const
{
	return (m_color == Piece::Color::White) ? 'k' : 'K';
}

char Queen::getLetter() const
{
	return (m_color == Piece::Color::White) ? 'q' : 'Q';
}

char Bishop::getLetter() const
{
	return (m_color == Piece::Color::White) ? 'b' : 'B';
}

char Knight::getLetter() const
{
	return (m_color == Piece::Color::White) ? 'n' : 'N';
}

char Rook::getLetter() const
{
	return (m_color == Piece::Color::White) ? 'r' : 'R';
}

Pawn::Pawn(const Coordinates& coordinates, Color color, bool hasMoved) : Piece{ coordinates, color, hasMoved } {}

Rook::Rook(const Coordinates& coordinates, Color color, bool hasMoved) : Piece{ coordinates, color, hasMoved } {}
Knight::Knight(const Coordinates& coordinates, Color color, bool hasMoved) : Piece{ coordinates, color, hasMoved } {}
Bishop::Bishop(const Coordinates& coordinates, Color color, bool hasMoved) : Piece{ coordinates, color, hasMoved } {}
Queen::Queen(const Coordinates& coordinates, Color color, bool hasMoved) : Piece{ coordinates, color, hasMoved } {}
King::King(const Coordinates& coordinates, Color color, bool hasMoved) : Piece{ coordinates, color, hasMoved } {}