#pragma once
#include <entt/entt.hpp>
#include <string>
#include <vector>

struct Account {
	std::string username;
	std::string password;
	int level;
	int experience;
	int gold;
	std::vector<std::string> mapsOpened;
};