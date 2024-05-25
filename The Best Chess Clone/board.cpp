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
#include <cmath>

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
	
	if (playerColor == Piece::Color::Black)
	{
		std::swap(m_matrix(0, 3), m_matrix(0, 4));
		std::swap(m_matrix(Constants::squaresPerLine - 1, 3), m_matrix(Constants::squaresPerLine - 1, 4));
	}

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
	constexpr auto isCastling{ [](Coordinates king, Coordinates move) { return abs(king.y - move.y) > 1; } };
	bool makeRookCastlingMove{ false };
	constexpr auto getPromotionRank{ [](Piece::Color player, Piece::Color promotion) { return player == promotion ? 0 : Constants::squaresPerLine - 1; } };

	if (Piece::isPiece(newSquare))
	{
		erasePieceFromList(newCoordinates);
	}
	else if (isEnPassant(newCoordinates, piece->getColor()) && piece->getType() == Piece::Type::Pawn)
	{	
		Coordinates rivalPawnCoordinates{ newCoordinates + Coordinates{ Piece::getForwardDirection(!piece->getColor()), 0 } };
		erasePieceFromList(rivalPawnCoordinates);
		m_matrix(rivalPawnCoordinates) = 'x';
	}
	else if (piece->getType() == Piece::Type::King && isCastling(oldCoordinates, newCoordinates))
	{
		makeRookCastlingMove = true;
	}

	m_enPassant = std::nullopt;

	if (!piece->hasMoved())
	{
		piece->addMovedFlag();

		if (piece->getType() == Piece::Type::Pawn)
			m_enPassant = EnPassant{ oldCoordinates + Coordinates{ Piece::getForwardDirection(piece->getColor()), 0 }, !piece->getColor() };
	}

	piece->getCoordinates() = newCoordinates;
	newSquare = m_matrix(oldCoordinates);
	m_matrix(oldCoordinates) = 'x';

	if (piece->getType() == Piece::Type::Pawn && getPromotionRank(m_playerColor, piece->getColor()) == newCoordinates.x)
	{
		auto& list{ getListFromColor(piece->getColor()) };
		char queenLetter{ piece->getColor() == Piece::Color::White ? 'q' : 'Q' };
		erasePieceFromList(newCoordinates);
		list.push_back(Piece::toPiece(queenLetter, newCoordinates, true));
	}
	
	if (makeRookCastlingMove)
	{
		int castlingDirection{ (newCoordinates > oldCoordinates) ? 1 : -1 };
		Coordinates rookCoordinates{ (castlingDirection == 1) ? Coordinates{oldCoordinates.x, Constants::squaresPerLine - 1 } : Coordinates{oldCoordinates.x, 0 } };
		Coordinates rookMove{ newCoordinates - Coordinates{ 0, castlingDirection } };
		makeMove(rookCoordinates, rookMove);
	}
}

std::vector<Coordinates> Board::getMoves(const Coordinates& coordinates)
{
	return getPieceFromList(coordinates)->getMoves(*this);
}

bool Board::isEnPassant(const Coordinates& coordinates, Piece::Color color) const
{
	return (m_enPassant) ? m_enPassant->coordinates == coordinates && m_enPassant->movingColor == color : false;
}

bool Board::isLegalMove(const Coordinates& oldCoordinates, const Coordinates& newCoordinates) const
{
	//todo: refactore and make it functional for en passants 

	char& currentPosition{ m_matrix(oldCoordinates) };
	char& attackedPosition{ m_matrix(newCoordinates) };
	const char currentLetter{ m_matrix(oldCoordinates) };
	const char attackedLetter{ m_matrix(newCoordinates) };

	const Piece::Color pieceColor{ Piece::getColor(currentLetter) };

	const auto& thisColorList{ getListFromColor(pieceColor) };
	const auto& rivalColorList{ getListFromColor(!pieceColor) };

	currentPosition = 'x';
	attackedPosition = currentLetter;

	std::vector<Piece*> attackingPieces{};
	attackingPieces.reserve(rivalColorList.size());
	
	for (const auto& piece : rivalColorList)
		if (piece->getCoordinates() != newCoordinates)
			attackingPieces.push_back(piece.get());

	const bool isKing{ Piece::getType(currentLetter) == Piece::Type::King };

	Coordinates kingCoordinates{};

	if (isKing) 
	{
		kingCoordinates = newCoordinates;
	}
	else
	{
		for (const auto& piece : thisColorList)
			if (piece->getType() == Piece::Type::King)
			{
				kingCoordinates = piece->getCoordinates();
				break;
			}
	}

	bool isLegal{ true };

	for (const auto& piece : attackingPieces)
		for (const auto& attack : piece->getAttacks(*this))
			if (attack == kingCoordinates)
			{
				isLegal = false;
				break;
			}
			
	currentPosition = currentLetter;
	attackedPosition = attackedLetter;

	return isLegal;
}

bool Board::isAttacked(const Coordinates& coordinates) const
{
	Piece::Color color{ Piece::getColor(m_matrix(coordinates)) };
	return isAttackedBy(coordinates, !color);
}

bool Board::isAttackedBy(const Coordinates& coordinates, Piece::Color color) const
{
	for (const auto& piece : getListFromColor(color))
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

bool Board::isStalemate(Piece::Color colorToPlay)
{
	if (isKingChecked(colorToPlay))
		return false;

	for (const auto& piece : getListFromColor(colorToPlay))
		for (const auto& move : piece->getMoves(*this))
			return false;

	return true;
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

	constexpr auto isCastling{ [](Coordinates king, Coordinates move) { return abs(king.y - move.y) > 1; } };
	constexpr auto getPromotionRank{ [](Piece::Color player, Piece::Color promotion) { return player == promotion ? 0 : Constants::squaresPerLine - 1; } };

	for (auto& piece : PiecesSavestate(thisColorList).load())
	{
		EvaluatedMove bestMove{};

		for (const auto& move : piece->getMoves(*this) )
		{
			PiecesSavestate initialPieceState{ thisColorList };
			PiecesSavestate initialRivalPieceState{ rivalColorList };
			EnPassantSavestate initialEnPassantState{ m_enPassant };

			Coordinates initialCoordinates{ piece->getCoordinates() };
			char& initialPosition{ m_matrix(initialCoordinates) };
			char& attackedPosition{ m_matrix(move) };
			const char initialLetter{ m_matrix(initialCoordinates) };
			const char attackedLetter{ m_matrix(move) };

			std::optional<BoardMatrix> currentMatrix{};
			
			if	(
					(isEnPassant(move, piece->getColor()) && piece->getType() == Piece::Type::Pawn) || 
					(piece->getType() == Piece::Type::King && isCastling(initialCoordinates, move)) ||
					(piece->getType() == Piece::Type::Pawn && getPromotionRank(m_playerColor, piece->getColor()) == move.x)
				)
			{
				currentMatrix = m_matrix;
			}

			makeMove(initialCoordinates, move);
			
			EvaluatedMove thisMove{ initialCoordinates, move };
			thisMove.eval = (deepness > 0) ? -getBestMoveForColor(!color, deepness - 1).eval : getColorEval(color);
			
			bestMove = max(bestMove, thisMove);
			
			if (currentMatrix)
			{
				m_matrix = currentMatrix.value();
			}
			else
			{
				initialPosition = initialLetter;
				attackedPosition = attackedLetter;
			}
			
			thisColorList = initialPieceState.load();
			rivalColorList = initialRivalPieceState.load();
			m_enPassant = initialEnPassantState.load();
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

Board::EnPassantSavestate::EnPassantSavestate(std::optional<Board::EnPassant> enPassant)
{
	save(enPassant);
}

void Board::EnPassantSavestate::save(std::optional<Board::EnPassant> enPassant)
{
	m_enPassant = enPassant;
}

std::optional<Board::EnPassant> Board::EnPassantSavestate::load()
{
	return m_enPassant;
}