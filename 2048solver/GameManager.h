#pragma once
#include "Grid.h"
#include "Vector.h"

class GameManager {
	int startTiles;
public:
	Grid grid;
	int score;
	bool over;
	bool won;

	GameManager();
	GameManager(Grid);
	bool addRandomTile();
	bool addTile(Position);
	bool move(int, bool);
	bool movesAvailable();
	void findFarthestPosition(Position, Vector, Position&, Position&);
	bool tileMatchesAvailable();
	bool positionsEqual(Position, Position);
	void printGrid();
	~GameManager();
};
