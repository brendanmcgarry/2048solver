#pragma once
#include "Header.h"
#include "Tile.h"
#include "Position.h"

class Grid {
	Tile cells[N][N];
public:
	Grid();
	Grid(int, int[][N]);
	int availableCells(Position[N*N]);
	Position randomAvailableCell();
	bool cellsAvailable();
	bool cellAvailable(Position);
	Tile cellContent(Position);
	void insertTile(Tile);
	void removeTile(Position);
	void merged(Position);
	void prepareTiles();
	void moveTile(Position, Position);
	bool withinBounds(Position);
	~Grid();
};
