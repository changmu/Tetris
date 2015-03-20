#include "tetris.h"

const string Square = "¡ö";
const string Space = "  ";
const string Line[] = {"¨T", "¨U", "¨T", "¨U"};
const string Corner[] = {"¨X", "¨[", "¨a", "¨^", "  "};
// const string Line2[] = {"©Ð", "©È", "©Ø", "©À"};
// const string Corner2[] = {"©°", "©´", "©¼", "©¸", "©à"};
const Point mainBoardPos(0, 0);		// Not logic on screen
const Point infoBoardPos(12, 0);	// Not logic on screen
const Point endBoardPos(0, 0);	// Not logic on screen


string Game::mainBoard[22][12];
string Game::infoBoard[22][6];
string Game::endBoard[22][18];
int Game::g_score = 0;

Game::Game() {
	HideCursor();
	srand(time(NULL));
	
	Block::initialize();
	nextBlock.pos.setPos(13, 3);
	nextBlock.kind = rand() % 7;
	nextBlock.dir = rand() % 4;

	/* Draw board */
	drawBoard((string *)mainBoard, 22, 12);
	drawBoard((string *)infoBoard, 22, 6);
	drawBoard((string *)endBoard, 22, 18);

	SetCursor(Point(0, 0));
	for (int i = 0; i < 22; ++i) {
		for (int j = 0; j < 12; ++j)
			cout << mainBoard[i][j];
		putchar('\n');
	}

	for (int i = 0; i < 22; ++i) {
		SetCursor(Point(infoBoardPos.x, infoBoardPos.y + i));
		for (int j = 0; j < 6; ++j)
			cout << infoBoard[i][j];
	}

	printAt(Point(13, 1), "Next:");
	printAt(Point(13, 7), "Score:");
	printAt(Point(13, 8), g_score);

	printAt(Point(13, 19), "@Changmu");
	printAt(Point(13, 20), "Ver 0.1");
}

void Game::drawBoard(string *b, int h, int w) {
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j) {
			if (i == 0) {
				if (j == 0) b[i * w + j] = Corner[0];
				else if (j == w - 1) b[i * w + j] = Corner[1]; 
				else b[i * w + j] = Line[0];
			} else if (i == h - 1) {
				if (j == 0) b[i * w + j] = Corner[3];
				else if (j == w - 1) b[i * w + j] = Corner[2]; 
				else b[i * w + j] = Line[2];
			} else {
				if (j == 0) b[i * w + j] = Line[3];
				else if (j == w - 1) b[i * w + j] = Line[1];
				else b[i * w + j] = Corner[4];
			}
		}
}

bool Game::getBlock() {
	runningBlock.dir = nextBlock.dir;
	runningBlock.kind = nextBlock.kind;
	runningBlock.pos.setPos(3, 1);
	runningBlock.show(true);

	nextBlock.show(false);
	nextBlock.dir = rand() % 4;
	nextBlock.kind = rand() % 7;
	nextBlock.show(true);

	int kind = runningBlock.kind;
	int dir = runningBlock.dir;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			int x = runningBlock.pos.x + j;
			int y = runningBlock.pos.y + i;		// Attention
			if (mainBoard[y][x] != Space && runningBlock.block[i][j])
				return false;
		}

	return true;
}

void Game::endGame() {
	for (int i = 0; i < 22; ++i) {
		SetCursor(Point(endBoardPos.x, endBoardPos.y + i));
		for (int j = 0; j < 18; ++j)
			cout << endBoard[i][j];
	}
	printAt(Point(7, 10), "GAME OVER");
	printAt(Point(7, 11), "SCORE:");
	printf(" %d", g_score);

	getch();		// Wait to quit
}


int Block::inter_ms = 300;		// Initialize
string Block::g_block[7][4];	// All kinds of blocks

void Block::initialize() {
	// The shape of 'I'
	g_block[0][0] = g_block[0][2] = "0100 0100 0100 0100";
	g_block[0][1] = g_block[0][3] = "0000 1111 0000 0000";
	// The shape of 'L'
	g_block[1][0] = "0100 0100 0110 0000";
	g_block[1][1] = "0000 0111 0100 0000";
	g_block[1][2] = "0000 0110 0010 0010";
	g_block[1][3] = "0000 0010 1110 0000";
	// The shape of "J"
	g_block[2][0] = "0010 0010 0110 0000";
	g_block[2][1] = "0000 0100 0111 0000";
	g_block[2][2] = "0000 0110 0100 0100";
	g_block[2][3] = "0000 1110 0010 0000";
	// The shape of "T"
	g_block[3][0] = "0111 0010 0000 0000";
	g_block[3][1] = "0010 0110 0010 0000";
	g_block[3][2] = "0010 0111 0000 0000";
	g_block[3][3] = "0100 0110 0100 0000";
	// The shape of "S"
	g_block[4][0] = "0010 0011 0001 0000";
	g_block[4][1] = "0011 0110 0000 0000";
	g_block[4][2] = g_block[4][0];
	g_block[4][3] = g_block[4][1];
	// The shape of "Z"
	g_block[5][0] = "0001 0011 0010 0000";
	g_block[5][1] = "0110 0011 0000 0000";
	g_block[5][2] = g_block[5][0];
	g_block[5][3] = g_block[5][1];
	// The shape of "#"
	g_block[6][0] = "0011 0011 0000 0000";
	g_block[6][1] = g_block[6][2] = g_block[6][3] = g_block[6][0];
}

void Block::show(bool sign) {
	stringToBool();
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (block[i][j])
				printAt(Point(pos.x + j, pos.y + i), sign ? Square : Space);
}

bool Block::dropDown(int _ms) {
	Sleep(_ms);
	show(false);
	char ch;
	while (kbhit()) {		// Whether there are key hit among waiting
		show(false);
		ch = getch();
		if (ch == -32) {	// Sign of the direction key
			ch = getch();
			if (ch == 75) {
				--pos.x;					// Left
				if (!isOk()) ++pos.x;
			} else if (ch == 77) {
				++pos.x;					// Right
				if (!isOk()) --pos.x;	
			} else if (ch == 80) {			// Down to the bottom
				while (isOk()) ++pos.y;
				--pos.y;
			} else if (ch == 72) 				// Up: rotate
				rotate();

			show(true);
		} else if (ch == 32) {
			getch();
		} else if (ch == 27) {
			printAt(Point(1, 1), "Sure to quit? (Y/N):");
			ch = getch();
			if (ch == 'Y' || ch == 'y') {
				printAt(Point(1, 2), "Good bye!");
				// Sleep(200);
				getch();
				exit(0);
			}
		}
	}
	show(false);
	++pos.y;
	printAt(Point(13, 8), Game::g_score);
	if (isOk()) {
		show(true);
		return true;
	}
	else {
		--pos.y;
		show(true);
		return false;
	}
}

bool Block::isOk() {
	stringToBool();

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			int x = pos.x + j;
			int y = pos.y + i;		// Attention
			if (block[i][j] && (x < 1 || y < 1 || x > 10 || y > 20))
				return false;
			if (Game::mainBoard[y][x] != Space && block[i][j])
				return false;
		}

	return true;
}

void Block::rotate() {
	if (++dir == 4) dir = 0;
	if (!isOk()) {
		for (int i = 1; i <= 2; ++i) {
			pos.x -= i;				// Adjust to a suitable place
			if (isOk()) return;
			pos.x += i * 2;
			if (isOk()) return;
			pos.x -= i;
		}
		if (--dir == -1) dir = 3;	// Can't adjust
	} 
}

void Block::stringToBool() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			if (g_block[kind][dir][i*5+j] == '1')
				block[i][j] = true;
			else block[i][j] = false;
		}
}

void Block::putOn() {
	stringToBool();

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			int x = pos.x + j;
			int y = pos.y + i;		
			if (block[i][j])
				Game::mainBoard[y][x] = Square;
		}	
}

void Block::wipeLine() {
	for (int i = 1; i <= 20; ++i) {
		if (countLineSquare(i) == 10) {
			++Game::g_score;
			for (int j = i - 1; j > 0; --j)
				moveLineDown(j);
			if (countLineSquare(1) > 0)
				cleanLine(1);
		}
	}
	SetCursor(Point(0, 0));
	for (int i = 0; i < 22; ++i) {
		for (int j = 0; j < 12; ++j)
			cout << Game::mainBoard[i][j];
		putchar('\n');
	}
}

int Block::countLineSquare(int row) {
	int cnt = 0;
	for (int i = 1; i <= 10; ++i)
		if (Game::mainBoard[row][i] == Square) ++cnt;
	return cnt;
}

void Block::moveLineDown(int row) {
	for (int i = 1; i <= 10; ++i)
		Game::mainBoard[row+1][i] = Game::mainBoard[row][i];
}

void Block::cleanLine(int row) {
	for (int i = 1; i <= 10; ++i)
		Game::mainBoard[row][i] = Space;
}

void SetCursor(const Point p) {				// Set position of cursor		
	COORD cd = {p.x * 2, p.y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

void HideCursor() {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;
	GetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);
	ConsoleCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);
}
