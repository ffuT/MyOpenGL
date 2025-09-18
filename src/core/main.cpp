#include "Game.h"
#include <iostream>

int main() {
	Game game = Game("MyOpenGL");
	game.Run();
	std::cout << "Runtime: " << game.GetLifetime() << "s" << std::endl;
	return 0;
}