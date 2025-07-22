#include "AnimationSystem.h"
#include <cassert>
#include <iostream>
#include "../components/EntityTags.h"

AnimationSystem::AnimationSystem(entt::registry& registry)
    : registry(registry), frameDuration(0.1f) {
    // Access dispatcher from registry context and connect to events
    if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
        (*dispatcher)->sink<AnimationChangeEvent>().connect<&AnimationSystem::changeAnimation>(*this);
    }
}


void AnimationSystem::update(float deltaTime) {
    auto view = registry.view<AnimationComponent, sf::Sprite>();

    for (auto [entity, animation, sprite] : view.each()) {

        auto it = animation.data->animations.find({ animation.currentState, animation.currentDirection });
        if (it == animation.data->animations.end()) continue;

        Animation* currentAnimation = &it->second;
        sprite.setTexture(*currentAnimation->texture);
        animation.timer += deltaTime;

        if (animation.timer >= currentAnimation->frameDuration) {
			std::cout << "Animation timer: " << animation.timer << std::endl;
            animation.timer -= currentAnimation->frameDuration;
            animation.currentFrame.x++;
            if (animation.currentFrame.x >= currentAnimation->frameCount.x) {
                animation.currentFrame.x = 0;
                animation.currentFrame.y++;
                if (animation.currentFrame.y >= currentAnimation->frameCount.y) {
                    animation.currentFrame.y = 0; // Reset to first row if needed
                }
			}
			std::cout << "Current frame: " << animation.currentFrame.x << std::endl;

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
		//auto view = registry.view<PlayerTag>();
  //      for (auto playerEntity : view) {
		//	std::cout << animation.currentFrame.x << " " << animation.currentFrame.y << std::endl;
		//}

    }
}

void AnimationSystem::changeAnimation(const AnimationChangeEvent& event) {
    if (!registry.valid(event.entity)) return;
    if (!registry.all_of<AnimationComponent, sf::Sprite>(event.entity)) return;

    AnimationComponent& animComp = registry.get<AnimationComponent>(event.entity);
    sf::Sprite& sprite = registry.get<sf::Sprite>(event.entity);

    auto it = animComp.data->animations.find({ event.newState, event.newDirection });
    if (it == animComp.data->animations.end()) return;

    Animation& animation = it->second;

    animComp.currentState = event.newState;
    animComp.currentDirection = event.newDirection;
    animComp.currentFrame = { 0, 0 };
    animComp.timer = 0.f;

    int frameWidth = static_cast<int>(animation.frameSize.x);
    int frameHeight = static_cast<int>(animation.frameSize.y);

    sf::IntRect frameRect({ 0, static_cast<int>(animation.startFrame.y * frameHeight) }, { frameWidth, frameHeight });

    sprite.setTexture(*animation.texture);
    sprite.setTextureRect(frameRect);
}