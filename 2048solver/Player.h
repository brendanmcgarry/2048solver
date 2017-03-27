#pragma once
#include <Random>
#include "GameManager.h"

using namespace std;

class Player {
	default_random_engine generator;
	GameManager game;

	int ptr;
	int movesList[2];
	int failOrder[2];
	int fails;

	int numMoves;
	GameManager saveGame;
	int bestScore;

	int moveSpeed;
public:
	Player(GameManager);
	void start();
	void start(int);
	void moveLoop();
	void sequenceMove();
	int guessMove();
	int predictMove();
	int probMove(int, bool);
	int randomSimStrength(GameManager, int, int);
	~Player();
};
