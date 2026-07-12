#pragma once

#include <unordered_map>
#include <algorithm>

#include "BaseComponent.hpp"

enum class Species {
    VOID,       // ERROR / unset

    ANIMAL,     // Most neutral animals
    BEAST,      // Predator types
    GOD,        // Debugging entity
    HUMAN,
    ITEM,       // Usually unmoving object
    PLANT,      // Smaller plants, usually food
    STONE,      // Stone resource
    TREE        // Larger plants, wood resource
};

enum class Attributes {
    AGILITY,    // Movement/combat dexterity
    MIND,       // Perception, intelligence
    SPIRIT,     // Mana usage
    VITALITY    // Strength, health
};

enum class Vitals {
    BLOOD,      // Hitpoints
    ENERGY,     // Action points
    HUNGER,
    MANA,
    THIRST
};

struct Attribute {
    Attributes type;
    int value = 0;
};

struct Vital {
    Vitals type;
    float limit = 0.0f;
    float value = 0.0f;

    Vital(Vitals type, float limit) : type(type), limit(limit), value(limit) {}

    bool IsDepleted() const { return value <= 0.0f; }
    bool IsUnder(float threshold) const { return value < threshold; }
    float Ratio() const { return limit > 0.0f ? value / limit : 0.0f; }

    // Returns true if this modification just depleted the vital
    bool Modify(float amount) {
        if (limit <= 0.0f) return false;
        value = std::clamp(value + amount, 0.0f, limit);
        return IsDepleted();
    }
};

class HealthComponent : public BaseComponent {
    public:
        HealthComponent(Species species = Species::ANIMAL) : species(species) {}

        Species species;
        int level = 1;
        float experience = 0.0f;

        void AddAttribute(Attributes type, int value) {
            attributes.emplace(type, Attribute{type, value});
        }
        void AddVital(Vitals type, float limit) {
            vitals.emplace(type, Vital(type, limit));
        }

        // Pointer return: nullptr means "this entity doesn't have this stat at all",
        // not "it has it at zero". Callers (e.g. a mana-drain effect) must check this
        // explicitly before acting.
        Attribute* GetAttribute(Attributes type) {
            auto it = attributes.find(type);
            return it != attributes.end() ? &it->second : nullptr;
        }
        Vital* GetVital(Vitals type) {
            auto it = vitals.find(type);
            return it != vitals.end() ? &it->second : nullptr;
        }

        // --- Mass / mobility hooks --- //
        // Mirrors MovementSystem's speed term from add_velocity(): missing Agility -> neutral speed.
        float SpeedMultiplier() {
            Attribute* agility = GetAttribute(Attributes::AGILITY);
            if (!agility) return 1.0f;
            return 1.0f + (agility->value - 1) * 0.025f;
        }

        // Mirrors MovementSystem::_get_vitality(): how much this entity resists being pushed.
        // Missing Vitality -> neutral mass (behaves like an unstatted obstacle).
        float EffectiveMass() {
            Attribute* vitality = GetAttribute(Attributes::VITALITY);
            if (!vitality) return 1.0f;
            return std::max(1.0f, (float)vitality->value);
        }

    private:
        std::unordered_map<Attributes, Attribute> attributes;
        std::unordered_map<Vitals, Vital> vitals;
};