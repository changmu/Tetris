#include "tetris.h"

int main() {
	Game game;
	while (game.getBlock()) {
		while (game.runningBlock.dropDown(Block::inter_ms))
			;
		game.runningBlock.putOn();
		game.runningBlock.wipeLine();
	}
	game.endGame();
}
