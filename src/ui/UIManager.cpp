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
		
		// Set visibility if specified
		if (elemJson.contains("visible")) {
			button->setVisible(elemJson["visible"]);
		}

		return button;
	}
	else if (type == "panel") {
		auto position = sf::Vector2f(elemJson["position"][0], elemJson["position"][1]);
		auto size = sf::Vector2f(elemJson["size"][0], elemJson["size"][1]);

		Panel* panel = new Panel(position, size);

		// Set visibility if specified
		if (elemJson.contains("visible")) {
			panel->setVisible(elemJson["visible"]);
		}
		
		// Optional origin setting from JSON
		if (elemJson.contains("origin")) {
			std::string originStr = elemJson["origin"];
			if (originStr == "topLeft") panel->setOrigin(PanelOrigin::TopLeft);
			else if (originStr == "topCenter") panel->setOrigin(PanelOrigin::TopCenter);
			else if (originStr == "topRight") panel->setOrigin(PanelOrigin::TopRight);
			else if (originStr == "centerLeft") panel->setOrigin(PanelOrigin::CenterLeft);
			else if (originStr == "center") panel->setOrigin(PanelOrigin::Center);
			else if (originStr == "centerRight") panel->setOrigin(PanelOrigin::CenterRight);
			else if (originStr == "bottomLeft") panel->setOrigin(PanelOrigin::BottomLeft);
			else if (originStr == "bottomCenter") panel->setOrigin(PanelOrigin::BottomCenter);
			else if (originStr == "bottomRight") panel->setOrigin(PanelOrigin::BottomRight);
		}

		// Store panel with ID if provided
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
		
		// Optional origin setting from JSON
		if (elemJson.contains("origin")) {
			std::string originStr = elemJson["origin"];
			if (originStr == "topLeft") textElement->setOrigin(TextOrigin::TopLeft);
			else if (originStr == "topCenter") textElement->setOrigin(TextOrigin::TopCenter);
			else if (originStr == "topRight") textElement->setOrigin(TextOrigin::TopRight);
			else if (originStr == "centerLeft") textElement->setOrigin(TextOrigin::CenterLeft);
			else if (originStr == "center") textElement->setOrigin(TextOrigin::Center);
			else if (originStr == "centerRight") textElement->setOrigin(TextOrigin::CenterRight);
			else if (originStr == "bottomLeft") textElement->setOrigin(TextOrigin::BottomLeft);
			else if (originStr == "bottomCenter") textElement->setOrigin(TextOrigin::BottomCenter);
			else if (originStr == "bottomRight") textElement->setOrigin(TextOrigin::BottomRight);
		}
		
		// Set visibility if specified
		if (elemJson.contains("visible")) {
			textElement->setVisible(elemJson["visible"]);
		}
		
		// Check if this is a dynamic text element
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
	sf::Vector2f viewPosition = view.getCenter() - view.getSize() / 2.f;
	sf::Vector2f viewCenter = view.getCenter();

	// Sync all elements (including dynamic texts since they're added to elements vector)
	for(auto& element : elements)
	{
		// Center pause panel in viewport
		if (auto it = namedElements.find("pausePanel"); it != namedElements.end() && it->second == element) {
			element->setDrawPosition(viewCenter + element->getPosition());
		} else {
			element->setDrawPosition(viewPosition + element->getPosition());
		}
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