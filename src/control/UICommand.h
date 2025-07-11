#pragma once

class UICommand 
{
public:
	virtual ~UICommand() = default;
	virtual void execute() = 0;
};