#include "EnemyLibrary.h"
#include <fstream>
#include <stdexcept>

// Helper to convert string to EnemyType
EnemyType stringToEnemyType(const std::string& str) {
    if (str == "Goblin") return EnemyType::Goblin;
    if (str == "Troll") return EnemyType::Troll;
    if (str == "Dragon") return EnemyType::Dragon;
    if (str == "Skeleton") return EnemyType::Skeleton;
    throw std::runtime_error("Unknown EnemyType: " + str);
}

bool EnemyLibrary::loadFromFile(const std::string& filename) {

}

const EnemyData& EnemyLibrary::getEnemyData(EnemyType type) const {
    
}

