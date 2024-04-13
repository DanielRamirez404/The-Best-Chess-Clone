#include "piece.h"
#include "coordinates.h"

Piece::Piece(const Coordinates& myCoordinates, Color myColor, Type myType)
	: coordinates{ myCoordinates }, color{ myColor }, type{ myType } {}