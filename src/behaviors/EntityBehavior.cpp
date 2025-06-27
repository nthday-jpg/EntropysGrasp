#include "../components/movementComponents.h"
#include "../components/EntityTags.h"
#include "../components/statComponent.h"
#include "../components/hitbox.h"
#include "EntityBehavior.h"
#include <vector>

using namespace std;

vector<entt::entity> createSpell(entt::registry& registry, entt::entity caster, SpellID spellID, const SpellLibrary& spellLibrary, int count) {
    SpellData spellData = spellLibrary.getSpell(spellID);
    Position position = registry.get<Position>(caster);
    //auto direction = registry.get<LookingDirection>(caster);

    if (spellData.behaviorType == BehaviorType::Orbit) 
    {
        position.x -= spellData.radius;
        position.y -= spellData.radius;
    }
    vector<entt::entity> spellEntities;
    vector<MovementDirection> directions = check(spellID, spellLibrary, count);
    for (int i = 0; i < count ; ++i) 
    {
        entt::entity spellEntity = registry.create();

        registry.emplace<SpellTag>(spellEntity);
        registry.emplace<SpellID>(spellEntity, spellID); // Pass spellID directly without creating a temporary object
        registry.emplace<Position>(spellEntity, position);
        registry.emplace<Attack>(spellEntity, spellData.damage);
        registry.emplace<Mana>(spellEntity, spellData.manaCost);
        registry.emplace<Speed>(spellEntity, spellData.speed);
        registry.emplace<MovementDirection>(spellEntity, directions[i]);
        registry.emplace<Size>(spellEntity, spellData.size);
        registry.emplace<Radius>(spellEntity, spellData.radius);
        registry.emplace<BehaviorType>(spellEntity, spellData.behaviorType);
        registry.emplace<SpellEffect>(spellEntity, spellData.effect);
        registry.emplace<Hitbox>(spellEntity, Hitbox(15.0f, 15.0f, 0.0f, 0.0f));
		registry.emplace<Resistance>(spellEntity, 0.5f); // Example resistance value

        spellEntities.push_back(spellEntity);
    }

    return spellEntities;
}

vector<MovementDirection> check(SpellID spellID, const SpellLibrary& spellLibrary, int count) {
	SpellData spellData = spellLibrary.getSpell(spellID);
	vector<MovementDirection> directions;
	if (spellData.behaviorType == BehaviorType::Orbit) {
		for (int i = 0; i < count; ++i) {
			float angle = i * 3.14159f / 4.0f; // 45 degrees in radians
			directions.emplace_back(cos(angle), sin(angle));
		}
	} else {
		directions.emplace_back(1.0f, 0.0f); // Default direction
	}
	return directions;
}