#pragma once

#include "Registry.hpp"
#include "../components/HealthComponent.hpp"
#include "../components/MateriaComponent.hpp"
#include "../components/MovementComponent.hpp"

using ComponentRegistry = Registry<MovementComponent, HealthComponent, MateriaComponent>;