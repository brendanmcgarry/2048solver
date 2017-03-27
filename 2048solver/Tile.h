#pragma once
#include "Position.h"

class Tile {
	Position previous;
public:
	Position position;
	int value;
	bool mergedFrom;

	Tile();
	Tile(Position, int);
	Tile(int, int, int);
	void savePosition();
	void updatePosition(Position);
	void updatePosition(int, int);
};
