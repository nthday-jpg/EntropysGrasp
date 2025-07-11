#pragma once
#include "hitbox.h"
#include <unordered_map>
#include <vector>

enum class TileType {
	ROCK,
	PILLAR,
	WALL,
	TREE,
	LAVA,
	TEMPLE,
	FOG,
	CHEST,
	SWAMP,
	LAKE,
	RIVER,
	CAVE,
	VOID,
	UNKNOWN // Default case for unrecognized tile types
};

inline const std::unordered_map<std::string, TileType> stringToTileTypeMap = {
	{"rock", TileType::ROCK},
	{"pillar", TileType::PILLAR},
	{"wall", TileType::WALL},
	{"tree", TileType::TREE},
	{"lava", TileType::LAVA},
	{"temple", TileType::TEMPLE},
	{"fog", TileType::FOG},
	{"chest", TileType::CHEST},
	{"swamp", TileType::SWAMP},
	{"lake", TileType::LAKE},
	{"river", TileType::RIVER},
	{"cave", TileType::CAVE},
	{"void", TileType::VOID},
	// Add more mappings as needed
};

inline TileType stringToTileType(const std::string& str) {
	auto it = stringToTileTypeMap.find(str);
	if (it != stringToTileTypeMap.end()) {
		return it->second;
	}
	return TileType::UNKNOWN; // Default case if not found
}

inline bool isWalkableTile(TileType type) {
	return type == TileType::ROCK || type == TileType::TREE || type == TileType::FOG
		|| type == TileType::SWAMP || type == TileType::LAKE || type == TileType::RIVER
		|| type == TileType::LAVA;
}

inline bool isInteractableTile(TileType type) {
	return type == TileType::CHEST || type == TileType::TEMPLE 
		|| type == TileType::CAVE || type == TileType::VOID;
}

inline bool isCollidableTile(TileType type) {
	return !isWalkableTile(type) && !isInteractableTile(type);
}