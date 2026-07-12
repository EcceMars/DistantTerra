#pragma once

#include <vector>
#include <algorithm>

#include "BaseComponent.hpp"

enum class Essence {
    VOID,
    DARK,
    LIGHT,

    AIR,
    EARTH,
    FIRE,
    WATER
};

enum class Materia {
    NONE,       // Fallback

    ANIMAL,     // Meat, bones etc.
    ENERGY,     // All types of energy (natural or magical)
    FRUIT,      // Rarely non-edible (in the case of herbivores)
    HERB,       // Smallest plants, usually food
    METAL,      // Metal resource
    PLANT,      // Small plants, usually source of food
    POISON,     // Toxic component
    STONE,      // Stone resource
    WOOD        // Large plants, usually resource
};

class MateriaComponent : public BaseComponent {
    public:
        MateriaComponent(Essence essence = Essence::EARTH, std::vector<Materia> materials = { Materia::NONE }) : essence(essence), materials(std::move(materials)) {}

        Essence essence;
        std::vector<Materia> materials;

        bool Equals(const MateriaComponent& other) const {
            return other.essence == essence && other.materials == materials;
        }

        bool Has(Materia type) const {
            return std::find(materials.begin(), materials.end(), type) != materials.end();
        }
};