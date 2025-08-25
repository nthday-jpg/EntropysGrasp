#include "AnimationSystem.h"
#include <cassert>
#include <iostream>
#include "../components/EntityTags.h"
#include "../../manager/AnimationManager.h"

AnimationSystem::AnimationSystem(entt::registry& registry)
    : registry(registry), frameDuration(0.1f) {}

void AnimationSystem::update(float deltaTime) {
    auto view = registry.view<AnimationComponent, StateComponent>();

    for (auto [entity, animation, state] : view.each()) {

        if (registry.all_of<EnemyTag>(entity)) {
			std::cout << static_cast<int>(animation.currentState) << std::endl;
        }
        // Cập nhật trạng thái và hướng hiện tại từ StateComponent
        animation.currentState = state.currentState;
        animation.currentDirection = state.currentDirection;

        auto it = AnimationManager::getInstance().getAnimationData(animation.name)->animations.find({ animation.currentState, animation.currentDirection });
        if (it == AnimationManager::getInstance().getAnimationData(animation.name)->animations.end()) continue;
        Animation* currentAnimation = &it->second;

        sf::Sprite* sprite;
        if (!registry.all_of<sf::Sprite>(entity))
        {
            int frameWidth = static_cast<int>(currentAnimation->frameSize.x);
            int frameHeight = static_cast<int>(currentAnimation->frameSize.y);

            sf::IntRect frameRect(
                sf::Vector2i(
                    static_cast<int>(animation.currentFrame.x * frameWidth),
                    static_cast<int>(currentAnimation->startFrame.y * frameHeight)
                ),
                sf::Vector2i(frameWidth, frameHeight)
            );
            sprite = new sf::Sprite(*currentAnimation->texture);
            sprite->setTextureRect(frameRect);
            sprite->setOrigin({frameRect.size.x/2.0f, frameRect.size.y/2.0f}); // Set origin to center
            registry.emplace<sf::Sprite>(entity, *sprite);
        }
        else {
            sprite = &registry.get<sf::Sprite>(entity);
		}

        animation.timer += deltaTime;
        if (animation.timer >= currentAnimation->frameDuration) {
            animation.timer -= currentAnimation->frameDuration;
            animation.currentFrame.x++;
            if (animation.currentFrame.x >= currentAnimation->frameCount.x) {
                animation.currentFrame.x = 0;
                animation.currentFrame.y++;
                if (animation.currentFrame.y >= currentAnimation->frameCount.y) {
                    animation.currentFrame.y = 0; // Reset to first row if needed
                }
			}

            int frameWidth = static_cast<int>(currentAnimation->frameSize.x);
            int frameHeight = static_cast<int>(currentAnimation->frameSize.y);

            sf::IntRect frameRect(
                sf::Vector2i(
                    static_cast<int>(animation.currentFrame.x * frameWidth),
                    static_cast<int>(currentAnimation->startFrame.y * frameHeight)
                ),
                sf::Vector2i(frameWidth, frameHeight)
            );

            sprite->setTextureRect(frameRect);
        }

        // Không cần replace sprite nữa
    }
}