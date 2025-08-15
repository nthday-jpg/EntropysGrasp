#include "WindowManager.h"

WindowManager::WindowManager() : window(nullptr) 
{
	createWindow(800, 600, "Default Window");
}

void WindowManager::createWindow(unsigned int width, unsigned int height, const std::string& title) {
    // Clean up existing window if any
    if (window)
    { 
        delete window;
    }

    window = new sf::RenderWindow(
        sf::VideoMode({ width, height }),
        title,
        sf::Style::Default
    );
}

sf::RenderWindow& WindowManager::getWindow() {
    if (!window) {
        throw std::runtime_error("Window has not been created yet");
    }
    return *window;
}

bool WindowManager::hasWindow() const {
    return window != nullptr;
}

sf::Vector2f WindowManager::mapPixelToCoords(const sf::Vector2i& coords) const
{
	if (!window) {
        throw std::runtime_error("Window has not been created yet");
	}
	return window->mapPixelToCoords(coords);
}

void WindowManager::setFramerateLimit(unsigned int limit) {
    if (window) window->setFramerateLimit(limit);
}

bool WindowManager::isOpen() const {
    return window && window->isOpen();
}

void WindowManager::close() {
    if (window) window->close();
}

void WindowManager::clear(const sf::Color& color) {
    if (window) window->clear(color);
}

void WindowManager::draw(const sf::Drawable& drawable) {
    if (window) window->draw(drawable);
}

void WindowManager::display() {
    if (window) window->display();
}

std::optional<sf::Event> WindowManager::pollEvent() {
    if (!window) return std::nullopt;

    if (const std::optional event = window->pollEvent())
    {
        return event;
    }
    return std::nullopt;
}

bool WindowManager::handleEvent(const std::optional<sf::Event>& event) {
    if (!window || !event.has_value()) return false;
    if (event->is<sf::Event::Closed>()) 
    {
        close();
		return true;
    }
    else if (event->is<sf::Event::Resized>())
    {
        // Handle window resize if needed
    }
    return false;
}

void WindowManager::reset() {
    if (window) {
        window->setView(window->getDefaultView());
    }
}