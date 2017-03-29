#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include "Player.h"

Player::Player(GameManager start) {
	game = start;

	ptr = 0;
	movesList[0] = 0;
	movesList[1] = 1;
	failOrder[0] = 3;
	failOrder[1] = 2;
	fails = 0;

	numMoves = 0;
	srand(time(NULL));
}

void Player::start(int depth) {
	cout << probMove(depth, true);
	return;
}

void Player::start() {
	//Position empties[N*N];
	//int cellsOpen = game.grid.availableCells(empties);
	//int depth = cellsOpen > 8 ? 3 : (cellsOpen > 5 ? 4 : 5);
	start(3);
}

void Player::moveLoop() {
	int dir = 0;
	game.printGrid();
	if (numMoves < 100) {
		sequenceMove();
	}
	else {
		dir = guessMove();
		//dir = predictMove();
	}
	cout << endl;
	switch (dir) {
	case 0: cout << "^"; break;
	case 1: cout << ">"; break;
	case 2: cout << "V"; break;
	case 3: cout << "<"; break;
	}
	cout << endl;
	Sleep(moveSpeed);
	game.printGrid();
	cout << endl << endl << numMoves << endl;
	system("pause");
	system("cls");
}

void Player::sequenceMove() {
	if (!game.move(movesList[ptr++], true))
		fails++;
	else
		fails = 0;

	ptr %= 2;

	if (fails == 2) {
		for (int f : failOrder)
			if (game.move(f, true))
				break;
		fails = 0;
		ptr = 0;
	}

	numMoves++;
}

int Player::guessMove() {
	uniform_int_distribution<int> distribution(0, 3);
	int direction = distribution(generator);
	int testMoves = 0;
	int baseScore = game.score;
	const int n = 60;

	int trials[4];
	int trialsCtr = 0;

	saveGame = game;
	if (!game.over) {
		for (int i = 0; i < 4; i++) {
			int avg = 0;
			if (game.move(i, false)) {
				avg = randomSimStrength(game, n, 100);
				game = saveGame;
			}

			trials[trialsCtr++] = avg;
		}

		int max = 0;
		for (int d = 0; d < 4; d++) {
			if (game.move(d, true)) {
				if (trials[d] > max) {
					max = trials[d];
					direction = d;
				}
				// If two moves are equal, randomly choose one to prevent movelocking
				else if (trials[d] == max) {
					if (distribution(generator) > 1) {
						max = trials[d];
						direction = d;
					}
				}
			}
			game = saveGame;
		}
	}

	return direction;
}

int Player::probMove(int simHeight, bool root) {
	numMoves++;

	if (!game.over) {
		GameManager save = game;
		GameManager movedGame;

		vector<int> moveStrength(4);

		vector<Position> candidateTiles;
		vector<int> candidateStrengths2(16);
		vector<int> candidateStrengths4(16);
		int subscore;

		uniform_int_distribution<int> distribution(0, 3);

		save = game;
		for (int i = 0; i < 4; i++) {
			if (!game.move(i, false))
				moveStrength[i] = 0;
			else {
				if (simHeight == 0)
					moveStrength[i] = game.score;
				else {
					movedGame = game;
					candidateTiles = game.grid.availableCells();

					for (int j = 0; j < candidateTiles.size(); j++) {
						Tile tile2(candidateTiles[j], 2);
						game.grid.insertTile(tile2);

						if (!game.movesAvailable())
							candidateStrengths2[j] = 0;
						else
							candidateStrengths2[j] = probMove(simHeight - 1, false);
						game = movedGame;

						Tile tile4(candidateTiles[j], 4);
						game.grid.insertTile(tile4);

						if (!game.movesAvailable())
							candidateStrengths4[j] = 0;
						else
							candidateStrengths4[j] = probMove(simHeight - 1, false);
						game = movedGame;
					}
					
					int avg = 0;
					for (int j = 0; j < candidateTiles.size(); j++) {
						avg += candidateStrengths2[j] * 0.9;
						avg += candidateStrengths4[j] * 0.1;
					}
					avg /= candidateTiles.size();

					moveStrength[i] = avg;

					game = save;
				}
			}
		}

		int max = 0;
		int direction = distribution(generator);
		for (int i = 0; i < moveStrength.size(); i++) {
			if (game.move(i, false) && moveStrength[i] > max) {
				max = moveStrength[i];
				direction = i;
			}
			game = save;
		}
		if (root) {
			//cout << "NumFnCalls: " << numMoves << endl;
			return direction;
		}
		else
			return max;
	}
	else {
		return 0;
	}
}

int Player::randomSimStrength(GameManager baseGame, int width, int depth) {
	uniform_int_distribution<int> distribution(0, 3);
	vector<int> scores;
	vector<int> numsOfMoves;
	int testMoves = 0;
	int baseScore = baseGame.score;

	GameManager game = baseGame;
	for (int i = 0; i < width; i++) {
		game.addRandomTile();
		if (!game.movesAvailable())
			game.over = true;

		while (!game.over && testMoves < depth) {
			if (game.move(distribution(generator), true))
				testMoves++;
		}

		scores.push_back(game.score - baseScore);
		numsOfMoves.push_back(testMoves);
		game = baseGame;
		testMoves = 0;
	}

	int avg = 0;
	for (auto score : scores)
		avg += score;

	avg /= width;

	/*int numFails = 0;
	for (auto&& moves : numsOfMoves)
	if (moves < 4)
	numFails++;

	avg *= (1 + width - numFails) / (1 + width);*/

	return avg;
}

Player::~Player() {}
