#include "WindowManager.h"

void WindowManager::createWindow(unsigned int width, unsigned int height, const std::string& title) {
    // Clean up existing window if any
    delete window;

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