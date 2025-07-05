#include "gameControl.h"
#include <iostream>

void Pause::execute()
{
	std::cout << "Game paused." << std::endl;
}

void Resume::execute()
{
}

void Restart::execute()
{
}

void Exit::execute()
{
	std::cout << "Game exited." << std::endl;
}