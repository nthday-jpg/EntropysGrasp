#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <optional>
#include <SFML/Window/Event.hpp>

//Usage
//WindowManager& windowMgr = WindowManager::getInstance();
//windowMgr.createWindow(800, 600, "My Game");

class WindowManager {
private:
    // Private constructor for singleton pattern
    WindowManager();

    // Destructor to clean up the window
    ~WindowManager() {
        if (window) {
            window->close();
            delete window;
            window = nullptr;
		}
    }

    // The main window instance owned by the manager
    sf::RenderWindow* window;

public:
    // Delete copy/move constructors and assignment operators
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
    WindowManager(WindowManager&&) = delete;
    WindowManager& operator=(WindowManager&&) = delete;

    // Get the singleton instance
    static WindowManager& getInstance() {
        static WindowManager instance;
        return instance;
    }

    // Create the window with specified parameters
    void createWindow(unsigned int width, unsigned int height, const std::string& title);

    // Get the window reference
    sf::RenderWindow& getWindow();

    // Check if window exists
    bool hasWindow() const;

	sf::Vector2f mapPixelToCoords(const sf::Vector2i& coords) const;

    // Common window operations
    void setFramerateLimit(unsigned int limit);

    bool isOpen() const;

    void close();

    void clear(const sf::Color& color = sf::Color::Black);

	void draw(const sf::Drawable& drawable);

    void display();

    // Forward event polling to the window
    std::optional<sf::Event> pollEvent();

	// Handle window specific events
	bool handleEvent(const std::optional<sf::Event>& event);
};