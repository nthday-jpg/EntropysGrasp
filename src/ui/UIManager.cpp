#include "UIManager.h"
#include "Panel.h"
#include "Button.h"
#include "../manager/WindowManager.h"
#include "../manager/FontManager.h"
#include <fstream>
#include <iostream>

UIManager::UIManager() : background(nullptr), backgroundTexture(nullptr)
{
}

UIManager::~UIManager()
{
	for (auto& element : elements)
	{
		delete element; 
	}
}


UIElement* UIManager::parseElement(const nlohmann::json& elemJson, sf::Font* font) {
	std::string type = elemJson["type"];

	if (type == "button") {
		std::string command = elemJson["command"];
		std::string text = elemJson["text"];
		auto position = sf::Vector2f(elemJson["position"][0], elemJson["position"][1]);
		int charSize = elemJson.value("characterSize", 30);

		return new Button(command, text, font, position, charSize);
	}
	else if (type == "panel") {
		auto position = sf::Vector2f(elemJson["position"][0], elemJson["position"][1]);
		auto size = sf::Vector2f(elemJson["size"][0], elemJson["size"][1]);

		Panel* panel = new Panel(position, size);

		if (elemJson.contains("children")) {
			for (const auto& child : elemJson["children"]) {
				UIElement* childElement = parseElement(child, font);
				panel->addElement(childElement); // You need to implement this method
			}
		}

		return panel;
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

void UIManager::setBackground(sf::Texture* texture)
{
	this->backgroundTexture = texture;
	this->background = new sf::Sprite(*texture);
}

void UIManager::syncUIWithViewport()
{
	sf::View view = WindowManager::getInstance().getWindow().getView();
	sf::Vector2f viewPosition = view.getCenter() - view.getSize() / 2.f;

	for(auto& element : elements)
	{
		element->setDrawPosition(viewPosition + element->getPosition());
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