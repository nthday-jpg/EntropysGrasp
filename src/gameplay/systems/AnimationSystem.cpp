#include "AnimationSystem.h"
#include <cassert>
#include <iostream>
#include "../components/EntityTags.h"
#include "../../manager/AnimationManager.h"

AnimationSystem::AnimationSystem(entt::registry& registry)
    : registry(registry), frameDuration(0.1f) 
{
    
}

void AnimationSystem::sinkEvents()
{
    // Access dispatcher from registry context and connect to events
    if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>())
    {
        (*dispatcher)->sink<AnimationChangeEvent>().connect<&AnimationSystem::changeAnimation>(*this);
    }
    else
    {
        std::cerr << "Dispatcher not found in registry context!" << std::endl;
		assert(false); 
    }
}

void AnimationSystem::update(float deltaTime) {
    auto view = registry.view<AnimationComponent, sf::Sprite>();

    for (auto [entity, animation, sprite] : view.each()) {

        auto it = AnimationManager::getInstance().getAnimationData(animation.name)->animations.find({ animation.currentState, animation.currentDirection });
        if (it == AnimationManager::getInstance().getAnimationData(animation.name)->animations.end()) continue;

        Animation* currentAnimation = &it->second;
        sprite.setTexture(*currentAnimation->texture);

        if (animation.duration > 0.f) {
            animation.duration -= deltaTime;
            if (animation.duration <= 0.f) {
                // hết thời gian -> revert về state trước đó
                animation.currentState = animation.prevState;
                animation.currentFrame = { 0, 0 };
                animation.timer = 0.f;

                // load lại animation cũ
                auto prevIt = AnimationManager::getInstance().getAnimationData(animation.name)->animations.find({ animation.currentState, animation.currentDirection });
                if (prevIt != AnimationManager::getInstance().getAnimationData(animation.name)->animations.end()) {
                    Animation* prevAnim = &prevIt->second;
                    int frameWidth = static_cast<int>(prevAnim->frameSize.x);
                    int frameHeight = static_cast<int>(prevAnim->frameSize.y);

                    sprite.setTexture(*prevAnim->texture);
                    sf::IntRect frameRect(
                        sf::Vector2i(
                            static_cast<int>(prevAnim->startFrame.x * frameWidth),
                            static_cast<int>(prevAnim->startFrame.y * frameHeight)
                        ),
                        sf::Vector2i(frameWidth, frameHeight)
                    );

                    sprite.setTextureRect(frameRect);
                }
                continue; // bỏ qua update frame lần này
            }
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

            sprite.setTextureRect(frameRect);
        }

        // Không cần replace sprite nữa
        registry.emplace_or_replace<AnimationComponent>(entity, animation);
    }
}

void AnimationSystem::changeAnimation(const AnimationChangeEvent& event) {
    if (!registry.valid(event.entity)) return;
    if (!registry.all_of<AnimationComponent, sf::Sprite>(event.entity)) return;

    AnimationComponent& animComp = registry.get<AnimationComponent>(event.entity);
    sf::Sprite& sprite = registry.get<sf::Sprite>(event.entity);

    auto it = AnimationManager::getInstance().getAnimationData(animComp.name)->animations.find({ event.newState, event.newDirection });
    if (it == AnimationManager::getInstance().getAnimationData(animComp.name)->animations.end()) return;

    Animation& animation = it->second;

    animComp.currentState = event.newState;
    animComp.currentDirection = event.newDirection;
    animComp.currentFrame = { 0, 0 };
    animComp.timer = 0.f;

    if (event.duration > 0.f) {
        animComp.prevState = animComp.currentState;
        animComp.duration = event.duration;
    }
    else {
        animComp.duration = 0.f;
    }

    int frameWidth = static_cast<int>(animation.frameSize.x);
    int frameHeight = static_cast<int>(animation.frameSize.y);

    sf::IntRect frameRect(
        { static_cast<int>(animation.startFrame.x * frameWidth)
        , static_cast<int>(animation.startFrame.y * frameHeight) },
        { frameWidth, frameHeight });

    sprite.setTexture(*animation.texture);
    sprite.setTextureRect(frameRect);
}