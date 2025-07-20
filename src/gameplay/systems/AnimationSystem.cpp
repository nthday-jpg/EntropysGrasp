#include "AnimationSystem.h"
#include <cassert>
#include <iostream>

void AnimationSystem::update(float deltaTime) {
    auto view = registry.view<AnimationComponent, sf::Sprite>();

    for (auto [entity, animation, sprite] : view.each()) {

        auto it = animation.data->animations.find({ animation.currentState, animation.currentDirection });
        if (it == animation.data->animations.end()) continue;

        Animation* currentAnimation = &it->second;
        animation.timer += deltaTime;

        if (animation.timer >= currentAnimation->frameDuration) {
            animation.timer -= currentAnimation->frameDuration;

            animation.currentFrame.x++;
            if (animation.currentFrame.x >= currentAnimation->frameCount.x) {
                animation.currentFrame.x = 0;
                animation.currentFrame.y++;
                if (animation.currentFrame.y >= currentAnimation->frameCount.y) {
                    animation.currentFrame.y = 0;
                }
            }

            int frameWidth = currentAnimation->texture->getSize().x / currentAnimation->frameCount.x;
            int frameHeight = currentAnimation->texture->getSize().y / currentAnimation->frameCount.y;

            sf::IntRect frameRect;
            frameRect.position = {
                static_cast<int>(animation.currentFrame.x * frameWidth),
                static_cast<int>(animation.currentFrame.y * frameHeight)
            };
            frameRect.size = { frameWidth, frameHeight };

            sprite.setTexture(*currentAnimation->texture);
            sprite.setTextureRect(frameRect);
			registry.emplace_or_replace<sf::Sprite>(entity, sprite);
        }
		registry.emplace_or_replace<AnimationComponent>(entity, animation);
    }
}

void AnimationSystem::changeAnimation(const AnimationChangeEvent& event) {
    if (!registry.valid(event.entity)) 
        return;
    if (!registry.all_of<AnimationComponent, sf::Sprite>(event.entity)) 
        return;

    AnimationComponent& animComp = registry.get<AnimationComponent>(event.entity);
    sf::Sprite& sprite = registry.get<sf::Sprite>(event.entity);

    auto it = animComp.data->animations.find({ event.newState, event.newDirection });
    if (it == animComp.data->animations.end()) return;

    Animation& animation = it->second;

    animComp.currentState = event.newState;
    animComp.currentDirection = event.newDirection;
    animComp.currentFrame = { 0, 0 };
    animComp.timer = 0.f;

    int frameWidth = animation.texture->getSize().x / animation.frameCount.x;
    int frameHeight = animation.texture->getSize().y / animation.frameCount.y;

    sf::IntRect frameRect;
    frameRect.position = { 0, 0 };
    frameRect.size = { frameWidth, frameHeight };

    sprite.setTexture(*animation.texture);
    sprite.setTextureRect(frameRect);
}
