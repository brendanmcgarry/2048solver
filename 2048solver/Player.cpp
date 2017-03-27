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

void Player::start() {
	GameManager baseGame = game;

	cout << guessMove();
	return;

	int numGames = 0;

	//system("cls"); // OP

	while (true) {
		while (!game.over)
			moveLoop();
		numGames++;

		//cout << "Game " << numGames << ": " << (game.won ? 1 : 0) << " (" << game.score << ", " << numMoves << ")" << endl; // OP

		//system("cls"); // OP
		//game.printGrid(); // OP
		cout << endl << "GAME SCORE: " << game.score << endl;
		cout << numMoves << endl;

		numMoves = 0;
		game = baseGame;
		system("pause");
	}
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

int Player::predictMove() {
	uniform_int_distribution<int> distribution(0, 3);

	GameManager saveGame = game;
	GameManager directionSave;

	Position empties[N*N];
	int numEmpties = 0;

	int avgs[4];
	int avgsCtr = 0;
	int testMoves;

	int direction = distribution(generator);

	for (int i = 0; i < 4; i++) {

		if (game.move(i, false)) {
			numEmpties = game.grid.availableCells(empties);

			int twos[N*N];
			int fours[N*N];

			int tileCtr = 0;
			int baseScore = game.score;

			directionSave = game;

			for (int j = 0; j < numEmpties; j++) {
				// spawn a 2: 90% weight
				// then reload game
				testMoves = 0;
				Tile two(empties[j], 2);
				game.grid.insertTile(two);

				/*while (!game.over && testMoves < 40) {
				game.move(distribution(generator), true);
				testMoves++;
				}

				twos[tileCtr] = game.score - baseScore;*/
				twos[tileCtr] = randomSimStrength(game, 20, 100);
				game = directionSave;


				// spawn a 4: 10% weight
				// then reload game
				testMoves = 0;
				Tile four(empties[j], 4);
				game.grid.insertTile(four);

				/*while (!game.over && testMoves < 40) {
				game.move(distribution(generator), true);
				testMoves++;
				}

				fours[tileCtr++] = game.score - baseScore;*/
				fours[tileCtr++] = randomSimStrength(game, 20, 100);
				game = directionSave;
			}

			int avg = 0;

			for (int j = 0; j < numEmpties; j++) {
				avg += twos[j] * 0.9 + fours[j] * 0.1;
			}

			avg /= numEmpties;
			avgs[avgsCtr++] = avg;

			game = saveGame;
		}
		else {
			avgs[avgsCtr++] = 0;
		}
	}

	int max = 0;
	for (int i = 0; i < 4; i++) {
		if (avgs[i] > max) {
			max = avgs[i];
			direction = i;
		}
	}

	game.move(direction, true);
	numMoves++;
	return direction;
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
