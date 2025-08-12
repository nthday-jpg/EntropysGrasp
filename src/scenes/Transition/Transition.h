#pragma once
#include "../Scene.h"
#include "../../ui/Text.h"
#include <SFML/Graphics.hpp>

class TransitionScene : public Scene {
private:
    float waitTime;
    float elapsedTime;
    std::string targetScene;

    // Visual elements for transition
    sf::RectangleShape fadeOverlay;

    sf::Font* font;
	Text loadingText;
    
    // Animation properties
    float fadeAlpha;
    bool fadeIn;
    bool fadeOut;
    bool waitPhase;

public:
    TransitionScene(sf::RenderWindow& window, const std::string& targetScene, float waitDuration = 2.0f);

    void load() override;
    void unload() override;
    bool handleEvent(const std::optional<sf::Event>& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    void setupVisuals();
    void updateFade(float deltaTime);
};