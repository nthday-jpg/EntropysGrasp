#pragma once
enum class HitboxType {
    Rectangle,
    Circle
};

//add getCenter 

struct Hitbox {
    HitboxType type;

    // Rectangle parameters
    float width = 0.0f;
    float height = 0.0f;


    // Circle parameter
    float radius = 0.0f;


    // offset from the entity's position
    float offsetX = 0.0f;
    float offsetY = 0.0f;


    // Rectangle constructor
    Hitbox(float w, float h, float offX = 0.0f, float offY = 0.0f)
        : type(HitboxType::Rectangle), width(w), height(h), offsetX(offX), offsetY(offY) {
    }


    // Circle constructor
    Hitbox(float r, float offX = 0.0f, float offY = 0.0f)
        : type(HitboxType::Circle), radius(r), offsetX(offX), offsetY(offY) {
    }

    bool operator==(const Hitbox& other) const {
        return (this == &other);
    }

};