#include "UIOrigin.h"
#include <stdexcept>

sf::Vector2f UIOriginHelper::calculateOrigin(const sf::Vector2f& size, UIOrigin origin) {
	switch (origin) {
	case UIOrigin::TopLeft:
		return sf::Vector2f(0.0f, 0.0f);
	case UIOrigin::TopCenter:
		return sf::Vector2f(size.x / 2.0f, 0.0f);
	case UIOrigin::TopRight:
		return sf::Vector2f(size.x, 0.0f);
	case UIOrigin::CenterLeft:
		return sf::Vector2f(0.0f, size.y / 2.0f);
	case UIOrigin::Center:
		return sf::Vector2f(size.x / 2.0f, size.y / 2.0f);
	case UIOrigin::CenterRight:
		return sf::Vector2f(size.x, size.y / 2.0f);
	case UIOrigin::BottomLeft:
		return sf::Vector2f(0.0f, size.y);
	case UIOrigin::BottomCenter:
		return sf::Vector2f(size.x / 2.0f, size.y);
	case UIOrigin::BottomRight:
		return sf::Vector2f(size.x, size.y);
	default:
		return sf::Vector2f(size.x / 2.0f, size.y / 2.0f); // Default to center
	}
}

UIOrigin UIOriginHelper::parseOriginString(const std::string& originStr) {
	if (originStr == "topLeft") return UIOrigin::TopLeft;
	else if (originStr == "topCenter") return UIOrigin::TopCenter;
	else if (originStr == "topRight") return UIOrigin::TopRight;
	else if (originStr == "centerLeft") return UIOrigin::CenterLeft;
	else if (originStr == "center") return UIOrigin::Center;
	else if (originStr == "centerRight") return UIOrigin::CenterRight;
	else if (originStr == "bottomLeft") return UIOrigin::BottomLeft;
	else if (originStr == "bottomCenter") return UIOrigin::BottomCenter;
	else if (originStr == "bottomRight") return UIOrigin::BottomRight;
	else {
		throw std::runtime_error("Unknown origin string: " + originStr);
	}
}

std::string UIOriginHelper::originToString(UIOrigin origin) {
	switch (origin) {
	case UIOrigin::TopLeft: return "topLeft";
	case UIOrigin::TopCenter: return "topCenter";
	case UIOrigin::TopRight: return "topRight";
	case UIOrigin::CenterLeft: return "centerLeft";
	case UIOrigin::Center: return "center";
	case UIOrigin::CenterRight: return "centerRight";
	case UIOrigin::BottomLeft: return "bottomLeft";
	case UIOrigin::BottomCenter: return "bottomCenter";
	case UIOrigin::BottomRight: return "bottomRight";
	default: return "center";
	}
}