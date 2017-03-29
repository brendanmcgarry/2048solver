# 2048solver
This is a 2048 C++ game and solver

Usage:
2048solver.exe [board array](, [depth])

where board array is a 2048 board of 16 comma-separated 2048 tiles (powers of 2 beginning with 2) and depth is the number of moves ahead

Currently only returns one recommended move for the given board state and then exits. This is because I'm using 2048solver.exe with a Node.js application that runs a 2048 bot that receives its recommended moves from the executable.
