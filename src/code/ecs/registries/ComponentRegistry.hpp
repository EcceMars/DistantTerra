#pragma once

#include "Registry.hpp"
#include "../components/AnimationComponent.hpp"
#include "../components/BehaviorComponent.hpp"
#include "../components/ColliderComponent.hpp"
#include "../components/HealthComponent.hpp"
#include "../components/MateriaComponent.hpp"
#include "../components/MovementComponent.hpp"
#include "../components/VisualComponent.hpp"
#include "../components/WorldComponent.hpp"

class ComponentRegistry : public Registry<AnimationComponent, BehaviorComponent, ColliderComponent, HealthComponent, MateriaComponent, MovementComponent, VisualComponent, WorldComponent> {};