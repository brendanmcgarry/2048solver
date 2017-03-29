#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Grid.h"
#include "Tile.h"

Grid::Grid() {}

Grid::Grid(int s, int mtx[][N]) {
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			cells[y][x] = Tile(x, y, mtx[y][x]);
		}
	}
}

Position Grid::randomAvailableCell() {
	std::vector<Position> empties;

	empties = availableCells();

	srand(time(NULL));
	return empties[rand() % empties.size()];
}

std::vector<Position> Grid::availableCells() {
	std::vector<Position> empties;
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			if (cells[y][x].value == 0)
				empties.push_back({ x, y });
		}
	}
	return empties;
}

bool Grid::cellsAvailable() {
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			if (cells[y][x].value == 0)
				return true;
		}
	}
	return false;
}

bool Grid::cellAvailable(Position pos) {
	return cells[pos.y][pos.x].value == 0;
}

Tile Grid::cellContent(Position pos) {
	return cells[pos.y][pos.x];
}

void Grid::insertTile(Tile tile) {
	cells[tile.position.y][tile.position.x] = tile;
}

void Grid::removeTile(Position pos) {
	cells[pos.y][pos.x].value = 0;
}

void Grid::merged(Position pos) {
	cells[pos.y][pos.x].mergedFrom = true;
}

void Grid::prepareTiles() {
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			cells[y][x].mergedFrom = false;
		}
	}
}

void Grid::moveTile(Position src, Position dest) {
	if (!(src.x == dest.x && src.y == dest.y)) {
		cells[dest.y][dest.x].value = cellContent(src).value;
		cells[src.y][src.x].value = 0;
	}
}

bool Grid::withinBounds(Position pos) {
	return pos.x >= 0 && pos.x < N &&
		   pos.y >= 0 && pos.y < N;
}

Grid::~Grid() {
	
}
