#include "UIManager.h"
#include "Button.h"
#include "Panel.h"
#include "../manager/WindowManager.h"
#include "../manager/FontManager.h"
#include <fstream>
#include <iostream>

UIManager::UIManager() : background(nullptr), backgroundTexture(nullptr)
{
}

UIManager::~UIManager()
{
	clear();
}

UIElement* UIManager::parseElement(const nlohmann::json& elemJson, sf::Font* font) {
	std::string type = elemJson["type"];

	if (type == "button") {
		std::string command = elemJson["command"];
		std::string text = elemJson["text"];
		auto position = sf::Vector2f(elemJson["position"][0], elemJson["position"][1]);
		int charSize = elemJson.value("characterSize", 30);

		Button* button = new Button(command, text, font, position, charSize);
		
		if (elemJson.contains("visible")) {
			button->setVisible(elemJson["visible"]);
		}
		
		// Unified origin parsing
		if (elemJson.contains("origin")) {
			std::string originStr = elemJson["origin"];
			button->setOrigin(UIOriginHelper::parseOriginString(originStr));
		}

		return button;
	}
	else if (type == "panel") {
		auto position = sf::Vector2f(elemJson["position"][0], elemJson["position"][1]);
		auto size = sf::Vector2f(elemJson["size"][0], elemJson["size"][1]);

		Panel* panel = new Panel(position, size);

		if (elemJson.contains("visible")) {
			panel->setVisible(elemJson["visible"]);
		}
		
		// Unified origin parsing
		if (elemJson.contains("origin")) {
			std::string originStr = elemJson["origin"];
			panel->setOrigin(UIOriginHelper::parseOriginString(originStr));
		}

		if (elemJson.contains("id")) {
			std::string id = elemJson["id"];
			namedElements[id] = panel;
		}

		if (elemJson.contains("children")) {
			for (const auto& child : elemJson["children"]) {
				UIElement* childElement = parseElement(child, font);
				panel->addElement(childElement);
			}
		}

		return panel;
	}
	else if (type == "text") {
		std::string text = elemJson["text"];
		auto position = sf::Vector2f(elemJson["position"][0], elemJson["position"][1]);
		int charSize = elemJson.value("characterSize", 30);
		
		Text* textElement = new Text(*font, text, position, charSize);
		
		// Unified origin parsing
		if (elemJson.contains("origin")) {
			std::string originStr = elemJson["origin"];
			textElement->setOrigin(UIOriginHelper::parseOriginString(originStr));
		}
		
		if (elemJson.contains("visible")) {
			textElement->setVisible(elemJson["visible"]);
		}
		
		if (elemJson.contains("id")) {
			std::string id = elemJson["id"];
			dynamicTexts[id] = textElement;
		}
		
		return textElement;
	}

	throw std::runtime_error("Unknown element type: " + type);
}

void UIManager::loadFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open: " << std::filesystem::absolute(filePath) << "\n";
		throw std::runtime_error("Could not open UI file: " + filePath);
	}

	nlohmann::json elemData;		
	file >> elemData;
	file.close();

	font = FontManager::getInstance().getFont(elemData["font"]);

	for (const auto& elem : elemData["elements"]) {
		addElement(parseElement(elem, font));
	}
}

void UIManager::bindDispatcher(entt::dispatcher* dispatcher)
{
	this->dispatcher = dispatcher;
}

void UIManager::clear()
{
	for (auto& element : elements)
	{
		delete element; 
	}
	elements.clear();
	
	// Clear the dynamic text maps since the elements are already deleted
	dynamicTexts.clear();
	textUpdaters.clear();
	namedElements.clear();
	
	if (background)
	{
		delete background;
		background = nullptr;
		backgroundTexture = nullptr;
	}
}

void UIManager::setBackground(sf::Texture* texture)
{
	this->backgroundTexture = texture;
	this->background = new sf::Sprite(*texture);
}

void UIManager::syncUIWithViewport()
{
	sf::View view = WindowManager::getInstance().getWindow().getView();
	sf::Vector2f viewPosition = view.getCenter() - view.getSize() / 2.f;  // Top-left corner
	sf::Vector2f viewCenter = view.getCenter();
	sf::Vector2f viewSize = view.getSize();

	// Sync all elements (including dynamic texts since they're added to elements vector)
	for(auto& element : elements)
	{
		// Special handling for pause panel - always center in viewport
		if (auto it = namedElements.find("pausePanel"); it != namedElements.end() && it->second == element) {
			element->setDrawPosition(viewCenter + element->getPosition());
		} 
		// Special handling for main menu panel - always center in viewport  
		else if (auto it = namedElements.find("mainMenuPanel"); it != namedElements.end() && it->second == element) {
			element->setDrawPosition(viewCenter + element->getPosition());
		}
		else {
			// Handle different UI element types with their origins
			sf::Vector2f anchorPoint = viewPosition; // Default to top-left
			
			// Check for Text elements
			Text* textElement = dynamic_cast<Text*>(element);
			if (textElement) {
				UIOrigin origin = textElement->getOrigin();
				anchorPoint = calculateAnchorPoint(viewPosition, viewCenter, viewSize, origin);
			}
			// Check for Panel elements  
			else {
				Panel* panelElement = dynamic_cast<Panel*>(element);
				if (panelElement) {
					UIOrigin origin = panelElement->getOrigin();
					anchorPoint = calculateAnchorPoint(viewPosition, viewCenter, viewSize, origin);
				}
				// Check for Button elements
				else {
					Button* buttonElement = dynamic_cast<Button*>(element);
					if (buttonElement) {
						UIOrigin origin = buttonElement->getOrigin();
						anchorPoint = calculateAnchorPoint(viewPosition, viewCenter, viewSize, origin);
					}
				}
			}
			
			element->setDrawPosition(anchorPoint + element->getPosition());
		}
	}
}

sf::Vector2f UIManager::calculateAnchorPoint(const sf::Vector2f& viewPosition, const sf::Vector2f& viewCenter, const sf::Vector2f& viewSize, UIOrigin origin)
{
	switch (origin) {
		case UIOrigin::TopLeft:
			return viewPosition;  // Top-left corner
		case UIOrigin::TopCenter:
			return sf::Vector2f(viewCenter.x, viewPosition.y);  // Top-center
		case UIOrigin::TopRight:
			return sf::Vector2f(viewPosition.x + viewSize.x, viewPosition.y);  // Top-right corner
		case UIOrigin::CenterLeft:
			return sf::Vector2f(viewPosition.x, viewCenter.y);  // Center-left
		case UIOrigin::Center:
			return viewCenter;  // Center
		case UIOrigin::CenterRight:
			return sf::Vector2f(viewPosition.x + viewSize.x, viewCenter.y);  // Center-right
		case UIOrigin::BottomLeft:
			return sf::Vector2f(viewPosition.x, viewPosition.y + viewSize.y);  // Bottom-left
		case UIOrigin::BottomCenter:
			return sf::Vector2f(viewCenter.x, viewPosition.y + viewSize.y);  // Bottom-center
		case UIOrigin::BottomRight:
			return sf::Vector2f(viewPosition.x + viewSize.x, viewPosition.y + viewSize.y);  // Bottom-right
		default:
			return viewPosition;  // Default to top-left
	}
}

void UIManager::draw(sf::RenderTarget& target) const
{
	sf::RenderStates states = sf::RenderStates::Default;
	if (background)
	{
		target.draw(*background, states);
	}
	for (auto& element : elements)
	{
		if (element->isVisible())
		{
			target.draw(*element, states);
		}
	}
}

void UIManager::addElement(UIElement* element)
{
	if (element)
	{
		elements.push_back(element);
	}
	else
	{
		throw std::runtime_error("Attempted to add a null UIElement to UIManager.");
	}
}

bool UIManager::handleEvent(const std::optional<sf::Event>& event)
{
	for (auto& element : elements)
	{
		if (element->handleEvent(event))
		{
			//Play click sound when any UI element handles the event
			if (dispatcher)
				dispatcher->trigger<PlaySoundEvent>({ "click", false });
			return true; // Event was handled by an element
		}
	}
	return false;
}

void UIManager::addDynamicText(const std::string& id, Text* textElement, std::function<std::string()> updater) {
	if (!textElement) {
		throw std::runtime_error("Cannot add null text element as dynamic text.");
	}
	
	dynamicTexts[id] = textElement;
	textUpdaters[id] = updater;
	addElement(textElement); 
}

void UIManager::updateDynamicTexts() {
	for (const auto& [id, updater] : textUpdaters) {
		if (auto it = dynamicTexts.find(id); it != dynamicTexts.end()) {
			it->second->setString(updater());
		}
	}
}

void UIManager::updateDynamicText(const std::string& id) {
	if (auto textIt = dynamicTexts.find(id); textIt != dynamicTexts.end()) {
		if (auto updaterIt = textUpdaters.find(id); updaterIt != textUpdaters.end()) {
			textIt->second->setString(updaterIt->second());
		}
	}
}

UIElement* UIManager::getElementByID(const std::string& id) {
	auto it = namedElements.find(id);
	return (it != namedElements.end()) ? it->second : nullptr;
}