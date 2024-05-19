#include "board.h"
#include "piece.h"
#include "coordinates.h"
#include "constants.h"
#include <algorithm>
#include <array>
#include <utility>
#include <vector>
#include <memory>
#include <cctype>
#include <optional>

Board::Board(Piece::Color playerColor)
	: m_playerColor{ playerColor }, m_matrix
	{
		{
			'R','N','B','Q','K','B','N','R',
			'P','P','P','P','P','P','P','P',
			'x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x',
			'p','p','p','p','p','p','p','p',
			'r','n','b','q','k','b','n','r',
		}
	}
{
	Piece::setPlayerColor(playerColor);

	m_whitePieces.reserve(Constants::piecesPerColor);
	m_blackPieces.reserve(Constants::piecesPerColor);
	
	for (int i = 0; i < Constants::squaresPerLine; i++)
	{
		for (int j = 0; j < Constants::squaresPerLine; j++)
		{
			char& letter{ m_matrix(i, j) };

			if (!Piece::isPiece(letter))
				continue;

			if (playerColor == Piece::Color::Black)
				letter = static_cast<char>((letter == toupper(letter)) ? tolower(letter) : (toupper(letter)));
			
			auto& list{ getListFromColor(Piece::getColor(letter)) };

			list.push_back(Piece::toPiece(letter, { i, j }, false));
		}
	}
}

std::vector<const Piece*> Board::getPieces()
{
	std::vector<const Piece*> pieces{};

	pieces.reserve(m_whitePieces.size() + m_blackPieces.size());

	for (const auto& piece : m_whitePieces)
		pieces.push_back(piece.get());

	for (const auto& piece : m_blackPieces)
		pieces.push_back(piece.get());

	return pieces;
}

Piece::Color Board::getPlayerColor() const
{
	return m_playerColor;
}

char Board::operator()(const Coordinates& coordinates) const
{
	return m_matrix(coordinates);
}

void Board::makeMove(const Coordinates& oldCoordinates, const Coordinates& newCoordinates)
{
	char& newSquare{ m_matrix(newCoordinates) };
	const auto& piece = getPieceFromList(oldCoordinates);

	if (Piece::isPiece(newSquare))
		erasePieceFromList(newCoordinates);
	else if (isEnPassant(newCoordinates) && piece->getType() == Piece::Type::Pawn)
		erasePieceFromList(newCoordinates + Coordinates{ Piece::getForwardDirection(!piece->getColor()), 0 });

	piece->getCoordinates() = newCoordinates;
	
	if (!piece->hasMoved())
	{
		piece->addMovedFlag();
		if (piece->getType() == Piece::Type::Pawn)
			m_enPassant = Coordinates{ oldCoordinates + Coordinates{ Piece::getForwardDirection(piece->getColor()), 0 }  };
	}

	newSquare = m_matrix(oldCoordinates);
	m_matrix(oldCoordinates) = 'x';
}

std::vector<Coordinates> Board::getMoves(const Coordinates& coordinates)
{
	return getPieceFromList(coordinates)->getMoves(*this);
}

bool Board::isEnPassant(const Coordinates& coordinates) const
{
	return (m_enPassant) ? m_enPassant == coordinates : false;
}

bool Board::isLegalMove(const Coordinates& oldCoordinates, const Coordinates& newCoordinates) const
{
	char& currentPosition{ m_matrix(oldCoordinates) };
	char& attackedPosition{ m_matrix(newCoordinates) };
	const char currentLetter{ m_matrix(oldCoordinates) };
	const char attackedLetter{ m_matrix(newCoordinates) };

	currentPosition = 'x';
	attackedPosition = currentLetter;

	const bool isKing{ Piece::getType(currentLetter) == Piece::Type::King };
	const bool isLegal{ isKing ? !isAttacked(newCoordinates) : !isKingChecked(Piece::getColor(currentLetter)) };

	currentPosition = currentLetter;
	attackedPosition = attackedLetter;

	return isLegal;
}

bool Board::isAttacked(const Coordinates& coordinates) const
{
	Piece::Color color{ Piece::getColor(m_matrix(coordinates)) };
	const auto& rivalList{ getListFromColor(!color) };

	for (const auto& piece : rivalList)
	{
		for (const auto& attack : piece->getAttacks(*this))
		{
			if (attack == coordinates)
				return true;
		}
	}

	return false;
}

void Board::erasePieceFromList(const Coordinates& coordinates)
{
	char letter{ m_matrix(coordinates) };
	auto& list{ getListFromColor(Piece::getColor(letter)) };

	auto piece
	{
		std::find_if(list.begin(), list.end(), [&](const std::unique_ptr<Piece>& piece)
		{
			return piece->getCoordinates() == coordinates;
		})
	};

	list.erase(piece);
}

Piece* Board::getPieceFromList(const Coordinates& coordinates)
{
	const char letter{ m_matrix(coordinates) };
	const auto& list{ getListFromColor(Piece::getColor(letter)) };

	auto piece
	{
		std::find_if(list.begin(), list.end(), [&](const std::unique_ptr<Piece>& piece)
		{
			return piece->getCoordinates() == coordinates;
		})
	};

	return piece->get();
}

bool Board::isFromPlayer(const Coordinates& coordinates) const
{
	const char letter{ m_matrix(coordinates.x, coordinates.y) };
	return Piece::isPiece(letter) && Piece::getColor(letter) == m_playerColor;
}

bool Board::isOutOfBounds(const Coordinates& coordinates)
{
	return coordinates < Coordinates{ 0, 0 } || coordinates > Coordinates{ Constants::squaresPerLine - 1, Constants::squaresPerLine - 1 };
}

const std::vector<std::unique_ptr<Piece>>& Board::getListFromColor(Piece::Color color) const
{
	return (color == Piece::Color::White) ? m_whitePieces : m_blackPieces;
}

std::vector<std::unique_ptr<Piece>>& Board::getListFromColor(Piece::Color color)
{
	return (color == Piece::Color::White) ? m_whitePieces : m_blackPieces;
}

bool Board::isKingChecked(Piece::Color color) const
{
	const auto& kingList{ getListFromColor(color) };

	auto king 
	{
		std::find_if(kingList.begin(), kingList.end(), [&](const std::unique_ptr<Piece>& piece)
			{
				return piece->getType() == Piece::Type::King;
			})
	};

	return isAttacked(king->get()->getCoordinates());
}

bool Board::isKingMated(Piece::Color color)
{
	const auto& kingList{ getListFromColor(color) };

	auto king
	{
		std::find_if(kingList.begin(), kingList.end(), [&](const std::unique_ptr<Piece>& piece)
			{
				return piece->getType() == Piece::Type::King;
			})
	};

	if (isAttacked(king->get()->getCoordinates()))
	{
		const auto movablePieceNumber
		{ 
			std::count_if(kingList.begin(), kingList.end(), [&](const std::unique_ptr<Piece>& piece)
				{ 
					return !piece->getMoves(*this).empty(); 
				})
		};

		return movablePieceNumber == 0;
	}

	return false;
}

void Board::makeAIMove()
{
	constexpr int defaultDeepness{ 1 };
	const EvaluatedMove bestMove { getBestMoveForColor(!m_playerColor, defaultDeepness) };
	makeMove(bestMove.initialCoordinates, bestMove.move);
}

Board::EvaluatedMove& Board::max(EvaluatedMove& firstMove, EvaluatedMove& secondMove)
{
	return (firstMove.eval > secondMove.eval) ? firstMove : secondMove;
}

Board::EvaluatedMove Board::getBestMoveForColor(Piece::Color color, int deepness)
{
	EvaluatedMove bestBranchMove{};

	auto& thisColorList{ getListFromColor(color) };
	auto& rivalColorList{ getListFromColor(!color) };

	for (auto& piece : PiecesSavestate(thisColorList).load())
	{
		EvaluatedMove bestMove{};

		for (const auto& move : piece->getMoves(*this) )
		{
			PiecesSavestate initialPieceState{ thisColorList };
			PiecesSavestate initialRivalPieceState{ rivalColorList };

			Coordinates initialCoordinates{ piece->getCoordinates() };
			char& initialPosition{ m_matrix(initialCoordinates) };
			char& attackedPosition{ m_matrix(move) };
			const char initialLetter{ m_matrix(initialCoordinates) };
			const char attackedLetter{ m_matrix(move) };
			makeMove(initialCoordinates, move);
			
			EvaluatedMove thisMove{ initialCoordinates, move };
			thisMove.eval = (deepness > 0) ? -getBestMoveForColor(!color, deepness - 1).eval : getColorEval(color);
			
			bestMove = max(bestMove, thisMove);
				
			initialPosition = initialLetter;
			attackedPosition = attackedLetter;
			
			thisColorList = initialPieceState.load();
			rivalColorList = initialRivalPieceState.load();
		}
		
		bestBranchMove = max(bestBranchMove, bestMove);
	}

	return bestBranchMove;
}

int Board::getColorEval(Piece::Color color)
{
	if (isKingMated(color))
		return Constants::minEval;

	if (isKingMated(!color))
		return Constants::maxEval;

	const auto& thisColorList{ getListFromColor(color) };
	const auto& rivalColorList{ getListFromColor(!color) };
	int eval{ 0 };

	for (const auto& piece : thisColorList)
	{
		eval += piece->getValue() * 100;

		for (int i{0}; i < piece->getAttacks(*this).size(); ++i)
			eval += 5;
	}

	for (const auto& piece : rivalColorList)
		eval -= piece->getValue() * 100;

	return eval;
}

Board::PiecesSavestate::PiecesSavestate(const std::vector<std::unique_ptr<Piece>>& piecesList)
{
	save(piecesList);
}

void Board::PiecesSavestate::save(const std::vector<std::unique_ptr<Piece>>& piecesList)
{
	m_pieces.reserve(piecesList.size());
	
	for (const auto& piece : piecesList)
		m_pieces.push_back(Piece::toPiece(piece->getLetter(), piece->getCoordinates(), piece->hasMoved()));
}

std::vector<std::unique_ptr<Piece>> Board::PiecesSavestate::load()
{
	return std::move(m_pieces);
}