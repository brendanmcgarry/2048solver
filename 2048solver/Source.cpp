#include <iostream>
#include <windows.h>
#include "GameManager.h"
#include "Player.h"
using namespace std;

void getMatrixFromArgs(char*[], int[][N]);
void printMatrix(int[][N]);
void printGrid(Grid);

int main(int argc, char* argv[]) {
	int mtx[N][N];

	// If one command-line argument is given (first is filename by default)
	if (argc == 2) {
		getMatrixFromArgs(argv, mtx);
		//printMatrix(mtx);
		//cout << endl << endl;

		Grid grid(N, mtx);
		GameManager game(grid);
		GameManager save;

		Player player(game);

		player.start();
	}
	else {
		cout << "Wrong number of arguments. Please pass " << N*N
			<< " comma-separated ints as an argument." << endl;
		system("pause");
		return 1;
	}
	
	//system("pause");
	return 0;
}

void getMatrixFromArgs(char *argv[], int mtx[][N]) {
	int tempCellVal = 0; // used to get each cell value from the cmd-line args

	for (int i = 0, j = 0; i < strlen(argv[1]) && j < N*N; i++) {
		if (argv[1][i] == ',') {
			mtx[j / N][j%N] = tempCellVal;
			tempCellVal = 0;
			j++;
		}
		else if (argv[1][i] >= '0' && argv[1][i] <= '9') {
			tempCellVal = tempCellVal * 10 + (argv[1][i] - '0');
		}
		else if (argv[1][i] != '[' && argv[1][i] != ']') {
			cout << "Invalid argument. Please enter " << N*N << " ints separated by commas." << endl;
			system("pause");
			exit;
		}
	}
	mtx[N - 1][N - 1] = tempCellVal;
}

void printMatrix(int mtx[][N]) {
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			cout << mtx[y][x] << " ";
		}
		cout << endl;
	}
}

void printGrid(Grid grid) {
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			cout << grid.cellContent({ x, y }).value << " ";
		}
		cout << endl;
	}
}