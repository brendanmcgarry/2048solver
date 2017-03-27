#include "Tile.h"
#include "Position.h"

Tile::Tile() {
	mergedFrom = false;
}

Tile::Tile(Position pos, int v) {
	position = pos;
	value = v;
	mergedFrom = false;
}

Tile::Tile(int x, int y, int v) {
	position.x = x;
	position.y = y;
	value = v;
	mergedFrom = false;
}

void Tile::savePosition() {
	previous = position;
}

void Tile::updatePosition(Position newPos) {
	position = newPos;
}

void Tile::updatePosition(int newX, int newY) {
	position.x = newX;
	position.y = newY;
}