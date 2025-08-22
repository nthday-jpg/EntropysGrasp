#include "Transition.h"
#include "../../manager/SceneManager.h"
#include "../../manager/FontManager.h"
#include "../../manager/WindowManager.h"
#include "../../GameConfig.h"
#include <iostream>
#include <algorithm>

TransitionScene::TransitionScene(sf::RenderWindow& window, const std::string& targetScene, float waitDuration)
    : Scene(window), targetScene(targetScene), waitTime(waitDuration), 
      elapsedTime(0.0f), fadeAlpha(0.0f), fadeIn(true), fadeOut(false), waitPhase(false),
      font(nullptr), loadingText(*FontManager::getInstance().getFont("default"), "Loading...", sf::Vector2f(0, 0), 48)
{
    setupVisuals();
}

void TransitionScene::setupVisuals()
{
    // Setup fade overlay
    sf::Vector2u windowSize = window.getSize();
    GameConfig& config = GameConfig::getInstance();
    fadeOverlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    fadeOverlay.setPosition({0,0});
    
    // Setup loading text
    font = FontManager::getInstance().getFont("default");
    if (font)
    {
        sf::View currentView = window.getView();
        sf::Vector2f viewCenter = currentView.getCenter();
        loadingText.setDrawPosition(viewCenter);
        loadingText.setCharacterSize(48);
        loadingText.setFillColor(sf::Color::White);
        loadingText.setVisible(false); // Initially hidden
    }
}

void TransitionScene::load()
{
    isLoaded = true;
    std::cout << "Transition scene loaded, transitioning to: " << targetScene << std::endl;
}

void TransitionScene::unload() 
{
    isLoaded = false;
}

bool TransitionScene::handleEvent(const std::optional<sf::Event>& event) 
{
    // Transition scene doesn't handle input events - it's non-interactive
    return false;
}

void TransitionScene::update(float deltaTime) 
{
    if (!isLoaded) 
    {
        load();
        return;
    }
    
    elapsedTime += deltaTime;
    
    // Phase 1: Fade in (0.5 seconds)
    if (fadeIn && !waitPhase && !fadeOut) 
    {
        updateFade(deltaTime);
        if (fadeAlpha >= 255.0f) 
        {
            fadeIn = false;
            waitPhase = true;
            elapsedTime = 0.0f; // Reset timer for wait phase
            loadingText.setVisible(true); // Show loading text
        }
    }
    // Phase 2: Wait (specified duration)
    else if (waitPhase && !fadeOut) 
    {
        if (elapsedTime >= waitTime)
        {
            waitPhase = false;
            fadeOut = true;
            elapsedTime = 0.0f; // Reset timer for fade out
            loadingText.setVisible(false); // Hide loading text
        }
    }
    // Phase 3: Fade out and transition (0.5 seconds)
    else if (fadeOut) 
    {
        updateFade(deltaTime);
        if (fadeAlpha <= 0.0f) 
        {
            // Transition complete, navigate to target scene using immediate navigation
            SceneManager::getInstance().navigateToImmediate(targetScene);
        }
    }
}

void TransitionScene::updateFade(float deltaTime) 
{
    const float fadeSpeed = 510.0f; // Alpha units per second (255 * 2 for 0.5 second fade)
    
    if (fadeIn || waitPhase) 
    {
        fadeAlpha = std::min(255.0f, fadeAlpha + fadeSpeed * deltaTime);
    } 
    else if (fadeOut)
    {
        fadeAlpha = std::max(0.0f, fadeAlpha - fadeSpeed * deltaTime);
    }
    
    std::uint8_t alpha = static_cast<std::uint8_t>(fadeAlpha);
    fadeOverlay.setFillColor(sf::Color(50, 50, 50, alpha));
    
    // Update text alpha during wait phase
    if (waitPhase && font) 
    {
        std::uint8_t textAlpha = static_cast<std::uint8_t>(std::min(255.0f, fadeAlpha));
        loadingText.setFillColor(sf::Color(255, 255, 255, textAlpha));
    }
}

void TransitionScene::render()
{
    // Clear with black background
    window.clear(sf::Color::Black);
    
    // Draw fade overlay
    window.draw(fadeOverlay);
    
    // Draw loading text during wait phase
    if (waitPhase && loadingText.isVisible())
    {
        window.draw(loadingText);
    } 
}