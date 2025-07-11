#include "Game.h"
#include <iostream>

using namespace std;

int main() {
	try
	{
		Game& game = Game::getInstance();
		game.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}