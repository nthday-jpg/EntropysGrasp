#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>

enum class UIOrigin {
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

class UIOriginHelper {
public:
	// Calculate origin offset based on size and origin type
	static sf::Vector2f calculateOrigin(const sf::Vector2f& size, UIOrigin origin);
	
	// Parse origin string from JSON to enum
	static UIOrigin parseOriginString(const std::string& originStr);
	
	// Convert enum to string (for debugging/serialization)
	static std::string originToString(UIOrigin origin);
};