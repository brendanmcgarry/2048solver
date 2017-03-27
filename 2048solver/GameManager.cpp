#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "GameManager.h"
#include "Traversal.h"

using namespace std;

GameManager::GameManager() {
	over = false;
	won = false;
	score = 0;
}

GameManager::GameManager(Grid g) {
	grid = g;
	srand(time(NULL));
	over = false;
	won = false;
	score = 0;
}

bool GameManager::addRandomTile() {
	if (grid.cellsAvailable()) {
		Position pos = grid.randomAvailableCell();
		Tile tile(pos.x, pos.y, ((rand() % 9) > 7) ? 4 : 2); // 90% chance of 2 tile vs 4 tile
		
		grid.insertTile(tile);
		
		return true;
	}
	else {
		return false;
	}
}

bool GameManager::addTile(Position pos) {
	if (grid.cellAvailable(pos)) {
		grid.insertTile(Tile(pos.x, pos.y, (rand() % 9 > 7) ? 4 : 2));
		return true;
	}
	else {
		return false;
	}
}

bool GameManager::move(int direction, bool randTile) {
	if (over) return false;

	Vector    vect(direction);
	Traversal traversal(vect);
	Tile      tile;
	Position  farthest;
	Tile      next({ -1, -1 }, 0);
	int       cellVal;
	int       nextVal;

	bool moved = false;

	grid.prepareTiles();

	for (int x : traversal.x) {
		for (int y : traversal.y) {
			tile.position = { x , y };
			cellVal = grid.cellContent({ x , y }).value;
			tile.value = cellVal;

			if (cellVal > 0) {
				findFarthestPosition(tile.position, vect, farthest, next.position); // updates farthest, next
				nextVal = (grid.withinBounds(next.position)) ? grid.cellContent(next.position).value : -1;
				next.mergedFrom = grid.cellContent(next.position).mergedFrom;

				if (nextVal != -1 && grid.cellContent(next.position).value == cellVal && !next.mergedFrom) {
					Tile merged(next.position, tile.value * 2);

					grid.insertTile(merged);
					grid.merged(next.position);
					grid.removeTile(tile.position);

					score += merged.value;
					if (merged.value == 2048)
						won = true;

					moved = true;
				}
				else {
					grid.moveTile(tile.position, farthest);
				}

				if (!positionsEqual(tile.position, farthest)) {
					moved = true;
				}
			}
		}
	}
	
	if (moved) {
		if (randTile)
			addRandomTile();
		
		if (!movesAvailable())
			over = true;
	}

	return moved;
}

void GameManager::findFarthestPosition(Position start, Vector vect, Position &previous, Position &cell) {
	cell = start;
	
	do {
		previous = cell;
		cell = { previous.x + vect.x, previous.y + vect.y };
	} while (grid.withinBounds(cell) &&
			 grid.cellAvailable(cell));
}

bool GameManager::movesAvailable() {
	return grid.cellsAvailable() || tileMatchesAvailable();
}

bool GameManager::tileMatchesAvailable() {
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			if (   ( (x < N-1) && grid.cellContent({ x, y }).value == grid.cellContent({ x + 1, y }).value )
				|| ( (y < N-1) && grid.cellContent({ x, y }).value == grid.cellContent({ x, y + 1 }).value ))
				return true;
		}
	}
	return false;
}

bool GameManager::positionsEqual(Position p1, Position p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

void GameManager::printGrid() {
	int magn = 0;
	int curL = 0;
	int val  = 0;

	// get string length of longest cell value
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			curL = log10(grid.cellContent({ x, y }).value) + 1;
			if (curL > magn)
				magn = curL;
		}
	}

	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			val = grid.cellContent({ x, y }).value;
			
			if (val == 0)
				curL = 1;
			else
				curL = log10(val) + 1;

			cout << grid.cellContent({ x, y }).value;
			
			// pad all cells to normalize string lengths
			for (int s = 0; s < 1 + magn - curL; s++) {
				cout << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

GameManager::~GameManager() {

}
