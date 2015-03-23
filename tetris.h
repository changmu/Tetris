#ifndef TETRIS_H
#define TETRIS_H

#include <windows.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

extern const string Square;
extern const string SquareFlash;
extern const string Space;
extern const string Line[];
extern const string Corner[];
// extern const string Line2[];
// extern const string Corner2[];

typedef struct Point Point;
extern const Point mainBoardPos;
extern const Point infoBoardPos;
extern const Point endBoardPos;


struct Point {
	int x, y;						// Column and row
	Point(int a = 0, int b = 0): x(a), y(b) {}
	void setPos(int a, int b) {
		x = a;
		y = b;
	}
};


class Block {
public:
	Point pos;						// On logic of mainBoard
	int kind, dir;					// Mean the global blocks
	bool block[4][4];				// On logic of itself
	static int inter_ms;			// Interval time between two drops
	static string g_block[7][4];

	static void initialize();
	void show(bool sign);			// Show or not
	bool dropDown(int _ms);
	bool isOk();					// To see whether this place is ok
	void rotate();
	void stringToBool();
	void putOn();					// Put the block on the mainBoard
    void flashLine(int row);
	void wipeLine();
	int countLineSquare(int row);
	void moveLineDown(int row);
	void cleanLine(int row);
};

class Game {
public:
	friend class Block;
	static int g_score;
	static string mainBoard[22][12];
	static string infoBoard[22][6];
	static string endBoard[22][18];
	void drawBoard(string *b, int h, int w);

	Block runningBlock, nextBlock;
	Game();
	bool getBlock();				// Get a block at the top
	void endGame();					// Summarize the score
};
void SetCursor(const Point p);		// Set position of cursor		

// Template's declaration and definition should be in the same header file
template<typename T>				
void printAt(Point pos, T obj) {
	SetCursor(pos);
	cout << obj;
}

void HideCursor();

#endif
