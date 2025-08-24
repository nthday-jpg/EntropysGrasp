#pragma once

class GameCommand
{
public:
	virtual ~GameCommand() = default;
	virtual void execute() = 0;
};